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
using namespace vex;

int tankDrive();
int splitArcadeWithFunction();
int splitArcadeBasic();
int clawControl();

int main()
{
    // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();
    // Deadband stops the motors when Axis values are close to zero.
    int deadband = 5;
    while (true)
    {
        // tankDrive();
         //splitArcadeWithFunction();
        splitArcadeBasic();
        clawControl();
    }
}

int clawControl()
{
    chainMotor.setStopping(brake); // prevent claw from falling down due to gravity

    clawMotor.setStopping(hold); // to hold objects
    clawMotor.setMaxTorque(35, percent); //don't burn out motor
    clawMotor.setTimeout(2, seconds);
     
    if (Controller1.ButtonL1.pressing())
    {
        chainMotor.spin(reverse, 50, percent); // move up at 50% speed
    }
    else if (Controller1.ButtonL2.pressing())
    {
        chainMotor.spin(forward, 50, percent); //move down at 50% speed
    }
    else
    {
        chainMotor.stop(); // stop the motor if the button is not pressed
    }

    if (Controller1.ButtonR1.pressing())
    {
        clawMotor.spin(reverse, 50, percent);
    }
    else if (Controller1.ButtonR2.pressing())
    {
        clawMotor.spin(forward, 50, percent);
    }
    else {
        clawMotor.stop();
    }

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
    int turnSpeed = Controller1.Axis1.position();
    int velocity = Controller1.Axis3.position();

    Brain.Screen.printAt(1, 30, "%d", velocity);
    Brain.Screen.printAt(1, 50, "%d", turnSpeed);
    wait(25, msec);
    Brain.Screen.clearScreen();

    int rightValue = (velocity - turnSpeed)/1.5;
    int leftValue = (velocity + turnSpeed)/1.5;

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