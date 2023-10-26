#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller;
inertial Inertial = inertial(PORT13);

motor driveFL = motor(PORT16, ratio6_1, true); // Verified
motor driveTL = motor(PORT14, ratio18_1, true); // Verified
motor driveBL = motor(PORT9, ratio6_1, true); // Verified
motor_group leftDrive = motor_group(driveFL, driveTL, driveBL);

<<<<<<< HEAD
motor driveFR = motor(PORT17, ratio6_1, false); // Verified
motor driveTR = motor(PORT3, ratio18_1, true); // Verified
motor driveBR = motor(PORT15, ratio6_1, false); // Verified
motor_group rightDrive = motor_group(driveFR, driveTR, driveBR);
=======
motor driveFR = motor(PORT16, ratio18_1, false);
motor driveBR = motor(PORT12, ratio18_1, false);
motor_group rightDrive = motor_group(driveFR, driveBR);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4

motor liftLeft = motor(PORT19, ratio36_1, true); // Verified
motor liftRight = motor(PORT20, ratio36_1, false); // Verified
motor_group lift = motor_group(liftLeft, liftRight);

motor puncher = motor(PORT5, ratio6_1, true); // Verified
motor triball_arm = motor(PORT1, ratio18_1, false);

smartdrive drive = smartdrive(leftDrive, rightDrive, Inertial, 12.56, 11.625, 10, distanceUnits::in);

<<<<<<< HEAD
rotation ForwardTrackingWheel = rotation(PORT7, true); // Verified
rotation SidewaysTrackingWheel = rotation(PORT2, true); // Verified
rotation LiftPotentiometer = rotation(PORT4, true); // Verified
=======
rotation ForwardTrackingWheel = rotation(PORT7, true);
rotation SidewaysTrackingWheel = rotation(PORT11, true);
rotation LiftPotentiometer = rotation(PORT5, true);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4

digital_out wingPneumatics = digital_out(Brain.ThreeWirePort.B);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}