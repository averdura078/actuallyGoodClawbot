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

drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 165, mm, 1); 

// VEXcode generated functions 
motor clawMotor = motor(PORT3, ratio18_1, false); //fix ports. ratio can either be 36:1, 18:1, or 6:1 (higher = more torque)
motor chainMotor = motor(PORT8, ratio18_1, false); 


// VEXcode generated functions 
/** 
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro. 
 * 
 * This should be called at the start of your int main function. 
 */ 

void vexcodeInit(void) { 

} 