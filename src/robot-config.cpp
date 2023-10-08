#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller;
inertial Inertial = inertial(PORT3);

motor driveFL = motor(PORT15, ratio18_1, true);
motor driveBL = motor(PORT9, ratio18_1, true);
motor_group leftDrive = motor_group(driveFL, driveBL);

motor driveFR = motor(PORT16, ratio18_1, false);
motor driveBR = motor(PORT12, ratio18_1, false);
motor_group rightDrive = motor_group(driveFR, driveBR);

motor liftLeft = motor(PORT20, ratio36_1, true);
motor liftRight = motor(PORT17, ratio36_1, false);
motor_group lift = motor_group(liftLeft, liftRight);

motor puncher = motor(PORT13, ratio6_1, true);
motor triball_arm = motor(PORT14, ratio18_1, false);

smartdrive drive = smartdrive(leftDrive, rightDrive, Inertial, 12.56, 11.625, 10, distanceUnits::in);

rotation ForwardTrackingWheel = rotation(PORT7, true);
rotation SidewaysTrackingWheel = rotation(PORT11, true);
rotation LiftPotentiometer = rotation(PORT5, true);

digital_out elevationPneumatics = digital_out(Brain.ThreeWirePort.B);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}