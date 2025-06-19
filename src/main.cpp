// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// left motors          motor         13, 19
// right motors         motor         9, 10
// clawMotor            motor         x
// chainMotor           motor         x
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robotconfig.h"
#include <stdio.h>
#include <math.h>
#include <cmath>
using namespace vex;

int tankDrive();
int splitArcadeWithFunction();
int splitArcadeBasic();
int clawControl();
int auton();

int chainUp();
int chainDown();
int clawClose();
int clawOpen();
int turn(float targetHeading);
int search();
int path();

int main()
{
    // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();
    Drivetrain.setStopping(brake);
    rotationSensor.resetPosition();
    inertialSensor.calibrate();
    while (inertialSensor.isCalibrating())
    {
        wait(100, msec);
    }

    while (true)
    {
        if (Controller1.ButtonX.pressing() == true) // run auton (for 30s)
        {
            // Brain.Screen.printAt(1, 90, "pressed");
            auton();
            Brain.Screen.clearScreen();
            Brain.Screen.setCursor(9, 1);
            Brain.Screen.print("DONE AUTON");
        }
        if (Controller1.ButtonA.pressing() == true) // run op control (for 1.5min)
        {
            Brain.Timer.reset();

            while (Brain.Timer.value() <= 90.0)
            {
                // tankDrive();
                splitArcadeBasic();
                // splitArcadeWithFunction();

                clawControl();
            }
        }
    }
}

int auton()
{
    /*
    PSEUDOCODE:

    - get preload over

    - drive to corner 1
    - pick up football 1
    - drive to wall and put over

    - drive to corner 2
    - pick up football 2
    - drive to wall and put over

    MAKE SURE IT DOESN'T TAKE MORE THAN 30s!!!

    FUNCTIONS:
    - chain go up
    - chain go down

    - claw close
    - claw open

    - turn with P controller

    - search
    - path
    */

    Brain.Timer.reset(); // resets timer to 0 immediately before timer starts counting to 30 seconds

    chainMotor.setStopping(hold);
    clawMotor.setStopping(hold);

    bool start = false;

    while (Brain.Timer.value() <= 30.0) // do autonomous routine
    {
        if (start == false)
        {
        Drivetrain.setDriveVelocity(4, percent);
        Drivetrain.driveFor(-50, mm, true);
        turn(180);
        Drivetrain.driveFor(-50, mm, true);

        clawOpen(); // drop off preload
        clawClose();

        turn(0);
        start = true;
        }

        // ALL AUTONOMOUS FUNCTIONS:
        // chainUp();
        // chainDown();
        // clawClose();
        // clawOpen();
        search();
        // path();

        int rotationValue = rotationSensor.position(degrees); // chain

        int distanceFromBall = ballDistanceSensor.value(); // in mm
        // Brain.Screen.printAt(60, 70, "%d", distanceFromBall);

        int distanceFromWall = wallDistanceSensor.value(); // in mm
        // Brain.Screen.printAt(60, 110, "%d", distanceFromWall);

        // P CONTROLLER
        // inertialSensor.resetHeading();
        // turn(180);

        // print stuff
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(inertialSensor.heading(degrees));
        Brain.Screen.setCursor(3, 1);
        Brain.Screen.print(rotationValue);
        Brain.Screen.setCursor(5, 1);
        Brain.Screen.print(distanceFromBall);
        Brain.Screen.setCursor(7, 1);
        Brain.Screen.print(distanceFromWall);
        Brain.Screen.clearScreen();
    }

    LeftDriveSmart.stop();
    RightDriveSmart.stop();
    Drivetrain.stop();
    clawMotor.stop();
    chainMotor.stop();

    return 0;
}

int chainUp()
{
    // while (rotationSensor.value() <1)
    // {
    //     chainMotor.spin(reverse, 50, percent);
    // }
    // chainMotor.stop(); // optional: stop motor after reaching target
    // return 0;

    chainMotor.setReversed(true);
    chainMotor.spinFor(2200, msec);
    Brain.Screen.print("going up");

    return 0;
}

// lower the chain until the rotation sensor reads 1 or more
int chainDown()
{
    // while (rotationSensor.value() >0)
    // {

    //     chainMotor.spin(forward, 50, percent);
    // }
    // chainMotor.stop(); // optional: stop motor after reaching target
    // return 0;

    chainMotor.setReversed(false);
    chainMotor.spinFor(2200, msec);
    Brain.Screen.print("going type MAYDAY");

    return 0;
}

// open the claw continuously
int clawOpen()
{
    // clawMotor.setTimeout(2, seconds);
    // clawMotor.spin(reverse, 50, percent);
    clawMotor.setReversed(true);
    clawMotor.spinFor(2000, msec);
    Brain.Screen.print("opening");

    // clawMotor.stop(); // ensure motor is stopped

    return 0;
}

// Function to close the claw continuously
int clawClose()
{
    // clawMotor.setTimeout(2, seconds);
    // clawMotor.spin(forward, 50, percent);
    clawMotor.setReversed(false);
    clawMotor.spinFor(2000, msec);
    Brain.Screen.print("closing");

    // clawMotor.stop(); // ensure motor is stopped

    return 0;
}

int search()
{
    int topVal = wallDistanceSensor.value();
    int bottVal = ballDistanceSensor.value();

    int buffer = 10;

    // while (topVal >= 1000 || bottVal >= 1000) // go forward to try to find anything
    // {
    //     topVal = wallDistanceSensor.value(); // update
    //     bottVal = ballDistanceSensor.value();

    //     LeftDriveSmart.spin(reverse, 4, pct); // forward
    //     RightDriveSmart.spin(reverse, 4, pct);

    //     Brain.Screen.setCursor(1, 1);
    //     Brain.Screen.print(topVal);
    //     Brain.Screen.setCursor(1, 2);
    //     Brain.Screen.print(bottVal);
    //     Brain.Screen.setCursor(1, 3);
    //     Brain.Screen.print("too far to see, going forward");
    //     wait(10, msec);
    // }

    while ((ballDistanceSensor.value() >= 1000) || (wallDistanceSensor.value() >= 1000) && Brain.Timer.value()<=30.0) // go forward if too far away
    {
        LeftDriveSmart.spin(reverse, 4, pct); // forward
        RightDriveSmart.spin(reverse, 4, pct);

        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(ballDistanceSensor.value());
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print("looking for anything");
        wait(10, msec);
        Brain.Screen.clearScreen();
    }

    while (abs(topVal - bottVal) >= buffer && Brain.Timer.value()<=30.0) // while different go forward
    {
        topVal = wallDistanceSensor.value(); // update
        bottVal = ballDistanceSensor.value();

        LeftDriveSmart.spin(reverse, 4, pct); // fwd
        RightDriveSmart.spin(reverse, 4, pct);

        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(topVal);
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print(bottVal);

        Brain.Screen.setCursor(3, 1);
        Brain.Screen.print("distance is different, going toward ball");

        wait(10, msec);
        Brain.Screen.clearScreen();

        if (ballDistanceSensor.value() <= 130)
        {
            LeftDriveSmart.stop();
            RightDriveSmart.stop();
            clawOpen();
            chainDown();
            clawClose();
            chainUp();

            turn(180);

            while (wallDistanceSensor.value() >= 30 && Brain.Timer.value()<=30.0)
            {
                LeftDriveSmart.spin(reverse, 5, percent);
                RightDriveSmart.spin(reverse, 5, percent);
            }
            LeftDriveSmart.stop();
            RightDriveSmart.stop();

            clawOpen();
            clawClose();

            turn(0);
        }
    }

    while (abs(topVal - bottVal) <= buffer && Brain.Timer.value()<=30.0) // distances are the same so spin
    {
        topVal = wallDistanceSensor.value(); // update
        bottVal = ballDistanceSensor.value();

        LeftDriveSmart.spin(reverse, 4, pct); // rotate
        RightDriveSmart.spin(forward, 4, pct);

        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(topVal);
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print(bottVal);

        Brain.Screen.setCursor(3, 1);
        Brain.Screen.print("distance is same, spinning to find ball");

        wait(10, msec);
        Brain.Screen.clearScreen();
    }

    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(topVal);
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print(bottVal);

    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("not in a while loop");

    wait(10, msec);
    Brain.Screen.clearScreen();

    return 0;
}

int turn(float targetHeading)
{
    float actualHeading = inertialSensor.heading(degrees);
    float error = targetHeading - actualHeading;
    int x = 0;

    if (error >= 0)
    {
        x = 0;
    }
    else if (error <= 0)
    {
        x = 0;
    }
    float motorSpeed = (0.15 * error) - x; // when error is big: motors fast, when error is small, motors slow

    while ((error > 2) || (error < -2))
    {

        if (fabs(error) < 5)
        {
            break;
        }
        actualHeading = inertialSensor.heading(degrees); // update current heading

        error = actualHeading - targetHeading; // update error, then make it always be between -180 and +180

        if (error > 180)
        {
            error -= 360;
        }
        if (error < -180)
        {
            error += 360;
        }

        motorSpeed = (0.15 * error) - x; // update speed

        // prevent motor burnout
        if (motorSpeed > 90)
        {
            motorSpeed = 90;
        }
        if (motorSpeed < -90)
        {
            motorSpeed = -90;
        }

        LeftDriveSmart.spin(forward, motorSpeed, pct);  // motorSpeed positive = spin fwd, motorSpeed negative = spin rev
        RightDriveSmart.spin(reverse, motorSpeed, pct); // motorSpeed negative = spin fwd, motorSpeed positive = spin rev

        // print all info. order: actual heading, error, motor speed
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(inertialSensor.heading(degrees));

        Brain.Screen.setCursor(3, 1);
        Brain.Screen.print(error);

        Brain.Screen.setCursor(5, 1);
        Brain.Screen.print(motorSpeed);

        wait(5, msec); // remove?
        // Brain.Screen.clearScreen();
    }

    Brain.Screen.setCursor(7, 1);
    Brain.Screen.print("OUT OF LOOP!");

    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    return 0;
}

int path()
{
    Drivetrain.setDriveVelocity(5, percent);
    Drivetrain.driveFor(200, mm, true);
    turn(180);
    Drivetrain.driveFor(200, mm, true);

    clawClose(); // drop off preload

    turn(0);

    while (wallDistanceSensor.value() > 900)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    turn(25);

    while (ballDistanceSensor.value() > 120)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    // --- Claw functions go here ---

    turn(180);

    while (wallDistanceSensor.value() > 20)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    turn(0);

    while (wallDistanceSensor.value() > 900)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    turn(270);

    while (wallDistanceSensor.value() > 300)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    turn(335);

    while (wallDistanceSensor.value() > 120)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    // --- Claw functions go here ---

    turn(180);

    while (wallDistanceSensor.value() > 20)
    {
        LeftDriveSmart.spin(reverse, 50, percent);
        RightDriveSmart.spin(reverse, 50, percent);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();

    return 0;
}

int clawControl()
{
    chainMotor.setStopping(brake); // prevent claw from falling down due to gravity

    clawMotor.setStopping(hold);         // to hold objects
    clawMotor.setMaxTorque(35, percent); // don't burn out motor
    clawMotor.setTimeout(2, seconds);

    if (Controller1.ButtonL1.pressing())
    {
        chainMotor.spin(reverse, 50, percent); // move up at 50% speed
    }
    else if (Controller1.ButtonL2.pressing())
    {
        chainMotor.spin(forward, 50, percent); // move down at 50% speed
    }
    else
    {
        chainMotor.stop(); // stop the motor if the button is not pressed
    }

    if (Controller1.ButtonR2.pressing())
    {
        clawMotor.spin(reverse, 50, percent);
    }
    else if (Controller1.ButtonR1.pressing())
    {
        clawMotor.spin(forward, 50, percent);
    }
    else
    {
        clawMotor.stop();
    }

    clawMotor.stop();
    chainMotor.stop();

    return 0;
}

int splitArcadeWithFunction()
{
    int turnSpeed = Controller1.Axis1.position();
    int velocity = Controller1.Axis3.position();

    Brain.Screen.printAt(1, 30, "%d", velocity);
    Brain.Screen.printAt(1, 50, "%d", turnSpeed);
    wait(25, msec);
    Brain.Screen.clearScreen();

    int rightValue = (0.00009 * (velocity - turnSpeed) * (velocity - turnSpeed) * (velocity + turnSpeed));
    int leftValue = (0.00009 * (velocity + turnSpeed) * (velocity + turnSpeed) * (velocity - turnSpeed));

    Brain.Screen.printAt(1, 90, "%d", rightValue);
    Brain.Screen.printAt(1, 110, "%d", leftValue);
    wait(25, msec);
    Brain.Screen.clearScreen();

    LeftDriveSmart.setVelocity(rightValue, percent);
    RightDriveSmart.setVelocity(leftValue, percent);

    LeftDriveSmart.spin(reverse);
    RightDriveSmart.spin(reverse);

    return 0;
}

int splitArcadeBasic()
{
    int turnSpeed;
    int velocity;
    int rightValue;
    int leftValue;

    turnSpeed = (Controller1.Axis1.position()) / 1.5;
    velocity = Controller1.Axis3.position();

    Brain.Screen.printAt(1, 30, "%d", velocity);
    Brain.Screen.printAt(1, 50, "%d", turnSpeed);
    wait(25, msec);
    Brain.Screen.clearScreen();

    rightValue = (velocity - turnSpeed) / 1.5;
    leftValue = (velocity + turnSpeed) / 1.5;

    Brain.Screen.printAt(1, 90, "%d", rightValue);
    Brain.Screen.printAt(1, 110, "%d", leftValue);
    wait(25, msec);
    Brain.Screen.clearScreen();

    LeftDriveSmart.setVelocity(leftValue, percent);
    RightDriveSmart.setVelocity(rightValue, percent);

    LeftDriveSmart.spin(reverse);
    RightDriveSmart.spin(reverse);

    return 0;
}

int tankDrive()
{
    int deadband = 5;

    // Get the velocity percentage of the left motor. (Axis3)
    int leftMotorSpeed = Controller1.Axis3.position();
    // Get the velocity percentage of the right motor. (Axis2)
    int rightMotorSpeed = Controller1.Axis2.position();

    // Set the speed of the left motor. If the value is less than the deadband, set it to zero.
    if (abs(leftMotorSpeed) < deadband)
    {
        // Set the speed to zero.
        LeftDriveSmart.setVelocity(0, percent);
    }
    else
    {
        // Set the speed to leftMotorSpeed
        LeftDriveSmart.setVelocity(leftMotorSpeed, percent);
    }

    // Set the speed of the right motor. If the value is less than the deadband, set it to zero.
    if (abs(rightMotorSpeed) < deadband)
    {
        // Set the speed to zero
        RightDriveSmart.setVelocity(0, percent);
    }
    else
    {
        // Set the speed to rightMotorSpeed
        RightDriveSmart.setVelocity(rightMotorSpeed, percent);
    }
    // Spin both motors in the forward direction.
    LeftDriveSmart.spin(forward);
    RightDriveSmart.spin(forward);
    wait(25, msec);

    return 0;
}

// old code chunks
// Drivetrain.setTurnVelocity(2, percent);
// while(inertialSensor.heading(degrees) < 90)
// {
//     Drivetrain.turn(left);
//     Brain.Screen.clearScreen();
//     Brain.Screen.setCursor(1, 10);
//     Brain.Screen.print(inertialSensor.heading(degrees));
// }
// while (inertialSensor.heading(degrees) > 90)
// {
//     Drivetrain.turn(right);
// }
// Drivetrain.stop();
////// Drivetrain.driveFor(reverse, 120, mm, false); // starts driving
//////if (rotationValue < 290)
// {
//     // chainMotor.spin(forward); // down is forward
// }
// else
// {
//     chainMotor.stop();
// }
