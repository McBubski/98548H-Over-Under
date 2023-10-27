#include "vex.h"
#include "Autonomous/odometry.h"
#include "Autonomous/autonomous.h"
#include "Autonomous/auton-functions.h"
#include "Graphics/graphics.h"
#include "Driver/drive-functions.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
  vexcodeInit();
  initializeRobot();
}

void autonomous(void) {
  leftDrive.setStopping(brake);
  rightDrive.setStopping(brake);
  drawGraphics = false;

  if (autonPath == 1) { // Home Auton
    homeAuton();
  } else if (autonPath == 2) {
    awayAuton();
  } else if (autonPath == 3) {
    skills();
  }
}

bool pneumaticsState = false;
bool puncherRunning = false;
bool liftMoving = false;
bool armDown = false;

void switchPneumatics() { // Toggle Pneumatics State
  if (pneumaticsState == false) {
    pneumaticsState = true;
  } else {
    pneumaticsState = false;
  }
}

void togglePuncher() { // Toggle Puncher State
   if (puncherRunning == false) {
    puncherRunning = true;
  } else {
    puncherRunning = false;
  }
}

void startLift() { // Begins arm lift to correct height.
  liftMoving = true;
}

void toggleArm() {
  if (armDown == false) {
    armDown = true;
  } else {
    armDown = false;
  }
}

void alignForDriverSkills() {
  driveFor(-6, 100);
  turnToHeading(237, 100);
}

void drivercontrol(void) {
  drawGraphics = true;
  leftDrive.setStopping(coast);
  rightDrive.setStopping(coast);

  puncher.setStopping(coast);
  lift.setStopping(brake);
  triball_arm.setStopping(brake);

  //Controller.ButtonA.pressed(switchPneumatics);
  Controller.ButtonB.pressed(toggleArm);
  Controller.ButtonL2.pressed(togglePuncher);
  Controller.ButtonUp.pressed(startLift);

  triball_arm.setVelocity(25, percent);

  while (1) {
    // Gets controller position in percent
    float joystickLeft = Controller.Axis3.position(pct); 
    float joystickRight = Controller.Axis2.position(pct);

    // Maps controller position to motors, also in percent
    leftDrive.spin(forward, joystickLeft, pct);
    rightDrive.spin(forward, joystickRight, pct);

    if (Controller.ButtonUp.pressing()) {
      driveFor(-6, 100);
      turnToHeading(237, 100);
    }


    if (Controller.ButtonR1.pressing()) { // Moves Lift Up
      lift.spin(forward, 100, percent);
    } else if (Controller.ButtonR2.pressing()) { // Moves Lift Down
      lift.spin(reverse, 100, percent);
    } else {
      lift.stop();
    }

    if (Controller.ButtonL2.pressing()) { // Puncher Controls
      puncher.spin(forward, 85, percent);
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

    if (Controller.ButtonL1.pressing()) {
      wingPneumatics.set(true);
    } else {
      wingPneumatics.set(false);
    }

    wait(20, msec);
  }
}


int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(drivercontrol);

  pre_auton();

  task odometryTask = task(positionTracking); // Starts Odometry
  task graphicsTask = task(updateScreen); // Starts Graphics

  while (true) {
    wait(100, msec);
  }
}
