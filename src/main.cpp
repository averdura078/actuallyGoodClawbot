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
            // tankDrive();
            // splitArcadeWithFunction();
            splitArcadeBasic();

            clawControl();
        }
    }
}

int auton()
{
    // while (Brain.Timer.value() <= 30.0)
    // {
    //     // auto code
    //     // make sure the movements do NOT take longer than 30 seconds
    // }
    // // stop all motors

    Brain.Timer.reset(); // resets timer to 0 immediately before timer starts counting to 30 seconds

    chainMotor.setStopping(hold);

    while (Brain.Timer.value() <= 30.0) //do auton
    {
        int rotationValue = rotationSensor.position(degrees);
        // Brain.Screen.printAt(60, 30, "%d", rotationValue);
        if (rotationValue < 290)
        {
            // chainMotor.spin(forward); // down is forward
        }
        else
        {
            chainMotor.stop();
        }

        int distanceFromBall = ballDistanceSensor.value(); // in mm
        // Brain.Screen.printAt(60, 70, "%d", distanceFromBall);

        int distanceFromWall = wallDistanceSensor.value(); // in mm
        // Brain.Screen.printAt(60, 110, "%d", distanceFromWall);


        
        // inertialSensor.resetHeading();
        // P CONTROLLER
        float targetHeading = 270;

        float actualHeading = inertialSensor.heading(degrees);
        float error = targetHeading - actualHeading;
        int x = 0;

        if (error >= 0)
        {
            x = 5;
        }
        else if (error <= 0)
        {
            x = -5;
        }
        float motorSpeed = (0.1 * error) - x; // when error is big: motors fast, when error is small, motors slow

        while ((error > 2) || (error < -2))
        {

            if (fabs(error) < 1)
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

            motorSpeed = (0.1 * error) - x; // update speed

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

            wait(5, msec);
            // Brain.Screen.clearScreen();
        }

        Brain.Screen.setCursor(7, 1);
        Brain.Screen.print("OUT OF LOOP!");

        LeftDriveSmart.stop();
        RightDriveSmart.stop();




        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(inertialSensor.heading(degrees));

        Brain.Screen.setCursor(3, 1);
        Brain.Screen.print(error);

        Brain.Screen.setCursor(5, 1);
        Brain.Screen.print(motorSpeed);

        Brain.Screen.clearScreen();
    }

    LeftDriveSmart.stop();
    RightDriveSmart.stop();
    Drivetrain.stop();
    clawMotor.stop();
    chainMotor.stop();

    return 0;
}

int clawControl()
{
    chainMotor.setStopping(brake); // prevent claw from falling down due to gravity

    clawMotor.setStopping(hold);         // to hold objects
    clawMotor.setMaxTorque(35, percent); // don't burn out motor
    clawMotor.setTimeout(2, seconds);

    while (Brain.Timer.value() <= 90.0)
    {

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

    while (Brain.Timer.value() <= 90.0)
    {
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
    }

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

//old code chunks

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