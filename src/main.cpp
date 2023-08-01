
#include "vex.h"
#include "graphics.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
  vexcodeInit();

  Inertial.calibrate();

  while(Inertial.isCalibrating()) {
    task::sleep(101);
  }
}

void autonomous(void) {
  
}

void usercontrol(void) {
  while (1) {
    wait(20, msec); 
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  task graphicsTask = task(updateScreen);

  while (true) {
    wait(100, msec);
  }
}
