
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
  wait(45, seconds);
  leftDrive.spin(forward, 50, percent);
  rightDrive.spin(forward, 50, percent);
  wait(10, seconds);
  leftDrive.spin(reverse, 100, percent);
  rightDrive.spin(reverse, 100, percent);
}

void usercontrol(void) {
  leftDrive.setStopping(coast);
  rightDrive.setStopping(coast);

  lift.setStopping(brake);

  while (1) {
    float joystickLeft = Controller.Axis3.position(percent);
    float joystickRight = Controller.Axis2.position(percent);

    leftDrive.spin(forward, joystickLeft, percent);
    rightDrive.spin(forward, joystickRight, percent);

    if (Controller.ButtonR2.pressing()) {
      lift.spin(forward, 100, percent);
    } else if (Controller.ButtonR1.pressing()) {
      lift.spin(reverse, 100, percent);
    } else {
      lift.stop();
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
