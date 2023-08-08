#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller;
inertial Inertial = inertial(PORT4);

motor catapultRight = motor(PORT7, ratio18_1, false);
motor catapultLeft = motor(PORT6, ratio18_1, true);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}