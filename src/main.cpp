
#include "vex.h"
#include "odometry.h"
#include "graphics.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
  vexcodeInit();

  Inertial.calibrate();

  while(Inertial.isCalibrating()) {    
    Brain.Screen.printAt(5, 15, "RevampedOS is launching...");
    Brain.Screen.printAt(5, 35, "glhf");

    task::sleep(100);

    Brain.Screen.clearScreen();
  }
}



void autonomous(void) {
  
}

bool cataSpinning = false;

void switchCata() {
  cataSpinning = !cataSpinning;
}

void usercontrol(void) {
  catapultLeft.setStopping(brake);
  catapultRight.setStopping(brake);
  Controller.ButtonA.pressed(switchCata);
  while (1) {
    if (Controller.ButtonR1.pressing()) {
      catapultLeft.spin(forward, 100, percent);
      catapultRight.spin(forward, 100, percent);
    } else {
      catapultLeft.stop();
      catapultRight.stop();
    }
    wait(20, msec); 
  }
}



int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  task odometryTask = task(positionTracking);
  task graphicsTask = task(updateScreen);

  while (true) {
    wait(100, msec);
  }
}
