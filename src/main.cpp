#include "vex.h"
#include "odometry.h"
#include "graphics.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
  vexcodeInit();

  Inertial.calibrate();

  while(Inertial.isCalibrating()) {    
    Brain.Screen.printAt(5, 15, "RevampedOS is launching..."); // Unneccesary, but looks cooler than it is
    Brain.Screen.printAt(5, 35, "glhf");

    task::sleep(100);

    Brain.Screen.clearScreen();
  }

  ForwardTrackingWheel.resetPosition();
  LiftPotentiometer.setPosition(5, degrees);
}


bool pneumaticsState = false;
bool puncherRunning = false;
bool liftMoving = false;
bool armDown = false;

void autonomous(void) {
  puncher.spin(forward, 80, percent);
}

void switchPneumatics() { // Toggle Pneumatics State
  pneumaticsState = !pneumaticsState;
}

void togglePuncher() { // Toggle Puncher State
  puncherRunning = !puncherRunning;
}

void startLift() { // Begins arm lift to correct height.
  liftMoving = true;
}

void toggleArm() {
  armDown = !armDown;
}


void drivercontrol(void) {
  leftDrive.setStopping(coast);
  rightDrive.setStopping(coast);

  puncher.setStopping(coast);
  lift.setStopping(brake);
  triball_arm.setStopping(brake);

  Controller.ButtonA.pressed(switchPneumatics);
  Controller.ButtonB.pressed(toggleArm);
  Controller.ButtonL2.pressed(togglePuncher);
  Controller.ButtonUp.pressed(startLift);

  triball_arm.setVelocity(25, percent);

  while (1) {
    float joystickLeft = Controller.Axis3.position(pct); // Drive Controls
    float joystickRight = Controller.Axis2.position(pct);

    leftDrive.spin(forward, joystickLeft, pct);
    rightDrive.spin(forward, joystickRight, pct);


    if (Controller.ButtonR1.pressing()) { // Lift Controls
      liftMoving = false;
      liftLeft.spin(forward, 12, volt);
      liftRight.spin(forward, 12, volt);
    } else if (Controller.ButtonR2.pressing()) {
      liftMoving = false;
      liftLeft.spin(reverse, 12, volt);
      liftRight.spin(reverse, 12, volt);
    } else {
      if (!liftMoving) {
        lift.stop();
      }
    }

    if (puncherRunning) { // Puncher Controls
      puncher.spin(forward, 75, percent);
    } else {
      puncher.stop();
    }

    if (!armDown) {
      if (LiftPotentiometer.angle() >= 5 && LiftPotentiometer.angle() <= 300) {
        triball_arm.spinTo(25, degrees, false);
      } else {
        triball_arm.spinTo(0, degrees, false);
      }
    } else {
      triball_arm.spinTo(140, degrees, false);
    }

    elevationPneumatics.set(pneumaticsState); // Wings State


    wait(20, msec);
  }
}


int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(drivercontrol);

  pre_auton();

  task odometryTask = task(positionTracking);
  task graphicsTask = task(updateScreen);

  while (true) {
    wait(100, msec);
  }
}
