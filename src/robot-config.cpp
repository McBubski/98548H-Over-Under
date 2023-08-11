#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller;
inertial Inertial = inertial(PORT4);

motor driveFL = motor(PORT11, ratio18_1, false);
motor driveML = motor(PORT12, ratio18_1, true);
motor driveBL = motor(PORT13, ratio18_1, false);
motor_group leftDrive = motor_group(driveFL, driveML, driveBL);

motor driveFR = motor(PORT14, ratio18_1, false);
motor driveMR = motor(PORT15, ratio18_1, true);
motor driveBR = motor(PORT16, ratio18_1, false);
motor_group rightDrive = motor_group(driveFR, driveMR, driveBR);
motor flyWheelTop = motor(PORT20, ratio36_1, false);
motor flyWheelBottom = motor(PORT17, ratio36_1, false);

motor intakeLeft = motor(PORT7, ratio6_1, false);
motor intakeRight = motor(PORT9, ratio6_1, true);

rotation ForwardTrackingWheel = rotation(PORT17, true);
rotation SideTrackingWheel = rotation(PORT16, true);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}