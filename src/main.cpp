// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// LeftMotor            motor         13, 19
// RightMotor           motor         9, 10
// ClawMotor            motor         x
// ArmMotor             motor         x
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robotconfig.h"
using namespace vex;

int tankDrive();
int splitArcade();

int main()
{
    // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();
    // Deadband stops the motors when Axis values are close to zero.
    int deadband = 5;
    while (true)
    {
        //tankDrive();
        splitArcade();
    }
}

int splitArcade()
{
    int turnSpeed = Controller1.Axis4.position();
    int velocity = Controller1.Axis2.position();

    Brain.Screen.printAt(1, 30, "%d", velocity);
    Brain.Screen.printAt(1, 50, "%d", turnSpeed);
    wait(25, msec);

    LeftDriveSmart.setVelocity(velocity - turnSpeed, percent);
    RightDriveSmart.setVelocity(velocity + turnSpeed, percent);

    LeftDriveSmart.spin(forward);
    RightDriveSmart.spin(forward);

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