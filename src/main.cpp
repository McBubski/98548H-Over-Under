
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

  ForwardTrackingWheel.resetPosition();
  SideTrackingWheel.resetPosition();
}



void autonomous(void) {
  
}

bool flywheelSpinning = false;

void switchCata() {
  flywheelSpinning = !flywheelSpinning;
}
void usercontrol(void) {
  leftDrive.setStopping(coast);
  rightDrive.setStopping(coast);

  intakeLeft.setStopping(coast);
  intakeRight.setStopping(coast);

  flyWheelBottom.setStopping(coast);
  flyWheelTop.setStopping(coast);

  Controller.ButtonL2.pressed(switchCata);

  while (1) {
    leftDrive.spin(forward, Controller.Axis3.position(percent), percent);
    rightDrive.spin(forward, Controller.Axis2.position(percent), percent);

    if (Controller.ButtonR2.pressing()) {
      intakeLeft.spin(forward, 100, percent);
      intakeRight.spin(forward, 100, percent);
    } else {
      intakeLeft.stop();
      intakeRight.stop();
    }

    if (flywheelSpinning) {
      flyWheelBottom.spin(forward, 100, percent);
      flyWheelTop.spin(forward, 100, percent);
    } else {
      flyWheelBottom.stop();
      flyWheelTop.stop();
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
