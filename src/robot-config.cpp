#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller;
inertial Inertial = inertial(PORT4);

motor driveFL = motor(PORT11, ratio18_1, true);
motor driveBL = motor(PORT13, ratio18_1, true);
motor_group leftDrive = motor_group(driveFL, driveBL);

motor driveFR = motor(PORT14, ratio18_1, false);
motor driveBR = motor(PORT15, ratio18_1, false);
motor_group rightDrive = motor_group(driveFR, driveBR);

motor liftLeft = motor(PORT20, ratio36_1, true);
motor liftRight = motor(PORT17, ratio36_1, false);
motor_group lift = motor_group(liftLeft, liftRight);

rotation ForwardTrackingWheel = rotation(PORT1, true);
rotation SideTrackingWheel = rotation(PORT3, true);

digital_out wingPneumatics = digital_out(Brain.ThreeWirePort.A);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}