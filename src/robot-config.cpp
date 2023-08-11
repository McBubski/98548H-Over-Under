#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller Controller;
inertial Inertial = inertial(PORT4);

motor flyWheel1 = motor(PORT16, ratio36_1, false);
motor flyWheel2 = motor(PORT17, ratio36_1, false);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}