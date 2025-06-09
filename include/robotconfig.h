#include "vex.h" 
using namespace vex; 
using signature = vision::signature; 
using code = vision::code; 

// A global instance of brain used for printing to the V5 brain screen 
brain Brain; 

// VEXcode device constructors 
controller Controller1 = controller(primary); 

// VEXcode device constructors 
motor leftMotorA = motor(PORT9, ratio18_1, false); 
motor leftMotorB = motor(PORT10, ratio18_1, false); 
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB); 

motor rightMotorA = motor(PORT19, ratio18_1, true);  
motor rightMotorB = motor(PORT13, ratio18_1, true);  
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB); 

//drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 310, 255, mm, 0.174); 

motor clawMotor = motor(PORT4, ratio18_1, false); //ratio can either be 36:1, 18:1, or 6:1 (higher = more torque)
motor chainMotor = motor(PORT8, ratio18_1, false); 

//sensors
rotation rotationSensor = rotation(PORT6, true);
distance ballDistanceSensor = distance(PORT15);
distance wallDistanceSensor = distance(PORT20);
inertial inertialSensor = inertial(PORT18);

smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, inertialSensor, 319.19, 310, 255, mm, 0.174);

void vexcodeInit(void) { 

} 