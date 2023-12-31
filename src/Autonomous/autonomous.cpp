#include "vex.h"
#include "Autonomous/autonomous.h"
#include "Autonomous/odometry.h"
#include "Autonomous/auton-functions.h"
#include "Autonomous/PID.h"
#include "Autonomous/pure-pursuit.h"

#include <vector>
#include <sstream>

int autonPath = 1;
int allianceColor = 1;

void initializeRobot() {
    Inertial.calibrate();

    while(Inertial.isCalibrating()) {    
        Brain.Screen.printAt(5, 15, "RevampedOS is launching..."); // Unneccesary, but looks cooler than it is
        Brain.Screen.printAt(5, 35, "glhf");

        task::sleep(100);

        Brain.Screen.clearScreen();
    }

    ForwardTrackingWheel.resetPosition();
    SidewaysTrackingWheel.resetPosition();
    LiftPotentiometer.setPosition(5, degrees);

    Inertial.setHeading(135, degrees);
}

void homeAuton() {
    //turnToHeading(0, 100);
    //followPath("Paths/DriveToGoalTest.txt");

    driveFor(-25, 100);
    turnToHeading(180, 90); // Align with Goal
//
    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
//
    wait(400, msec);
//
    leftDrive.stop();
    rightDrive.stop();
    //
    driveFor(5, 100);
    driveTo(-61, -41, 100, forward);
//
    turnToHeading(328, 100);
    //
    ballArm.set(true);
//
    driveFor(-15, 100);
    turnToHeading(285, 100);
    ballArm.set(false);
    driveFor(-19, 100);
    turnToHeading(269, 100);
    driveFor(-21, 100);
    turnToHeading(180, 100);
    driveFor(-2, 100);
    ballArm.set(true);
    turnToHeading(155, 100);

    // OLD

    //turnToHeading(325, 100);
    //ballArm.set(true);
    //driveFor(-12, 90);
    //turnToHeading(275, 100);
//
    //driveFor(-38, 100);
    //ballArm.set(false);
    //turnToHeading(135, 100);
    //ballArm.set(true);
//
    //driveFor(4, 100);
//
    //pointAt(-46, -46, 100); // Align To Matchload
    //driveFor(16, 100);
//
    //turnToHeading(45, 100);
    //leftDrive.spin(reverse, 15, percent);
    //rightDrive.spin(reverse, 15, percent);
    //wait(1500, msec);
    //leftDrive.stop();
    //rightDrive.stop();
//
    //triball_arm.spinTo(140, degrees, true);
    //driveFor(10, 100);
//
    //triball_arm.spinTo(25, degrees, false);
    //turnToHeading(135, 100);
//
    //driveFor(20, 100);
//
    //turnToHeading(90, 100);
    //lift.spinTo(390, degrees, false);
    //driveFor(18, 50);
}

void awayAuton() {
    turnToHeading(235, 100);
    ballArm.set(true);
    driveFor(-14, 100);
    turnToHeading(195, 100);
    turnToHeading(225, 100);
    driveFor(-5, 100);
    turnToHeading(180, 100);
    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(1500, msec);
    leftDrive.stop();
    rightDrive.stop();
    ballArm.set(false);
    driveFor(6, 100);


    //pointAt(-46, -46, 100); // Align To Matchload
    //driveFor(16, 100);

    //turnToHeading(225, 100);
    //leftDrive.spin(reverse, 15, percent);
    //rightDrive.spin(reverse, 15, percent);
    //wait(1500, msec);
    //leftDrive.stop();
    //rightDrive.stop();

    //triball_arm.spinTo(140, degrees, true);
    //driveFor(10, 100);

    //triball_arm.spinTo(25, degrees, false);
    //turnToHeading(315, 100);

    //driveFor(20, 100);

    //turnToHeading(270, 100);
    //lift.spinTo(390, degrees, false);
    //driveFor(18, 50);  
}
void skills() {
    driveFor(-25, 100);
    turnToHeading(180, 90); // Align with Goal
//
    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
//
    wait(400, msec);
//
    leftDrive.stop();
    rightDrive.stop();

    driveTo(-52.5, -51.5, 100, forward);
    turnToHeading(242.5, 100);

    puncher.spin(forward, 95, percent);
    wait(31, seconds);
    puncher.stop();

    driveTo(-38, -60, 100, reverse);
    driveTo(32, -60, 100, reverse);
    driveTo(32, -40, 100, reverse);
    driveTo(10, -26, 100, reverse);
    driveTo(10, 0, 100, reverse);

    turnToHeading(270, 100);

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(800, msec);
    leftDrive.stop();
    rightDrive.stop();

    driveTo(10, -2, 100, forward);
    driveTo(8, -33, 100, reverse);

    turnToHeading(230, 100);

    wingPneumatics.set(true);

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(1100, msec);
    leftDrive.stop();
    rightDrive.stop();

    wingPneumatics.set(false);

    //driveFor(6, 100);
    driveTo(9, -2, 100, forward);

    driveTo(8, 27, 100, reverse);
    turnToHeading(310, 100);

    wingPneumatics.set(true);

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(1100, msec);
    leftDrive.stop();
    rightDrive.stop();

    wingPneumatics.set(false);  
    driveFor(6, 100);

    driveTo(22, 0, 100, forward);

    turnToHeading(270, 100);

    wingPneumatics.set(true);

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(800, msec);
    leftDrive.stop();
    rightDrive.stop();   

    wingPneumatics.set(false);
    driveFor(6, 100);

   //driveTo(36, 60, 100, reverse);
   //driveTo(60, 40, 100, reverse);

   //turnToHeading(0, 100);

   //leftDrive.spin(reverse, 100, percent);
   //rightDrive.spin(reverse, 100, percent);
   //wait(650, msec);
   //leftDrive.stop();
   //rightDrive.stop();

   //driveFor(6, 100);

    // ASDASDASD

    //driveFor(-6, 100);
    //turnToHeading(240, 100);
    //driveFor(1, 100);
////
    //puncher.spin(forward, 85, percent);
    //wait(35, seconds);
    //puncher.stop();
////
    //driveFor(-4, 100);
    //turnToHeading(305, 100);
////
    //driveFor(-10, 100);
    //turnToHeading(269, 100);
////
    //driveFor(-72, 100);
    //turnToHeading(0, 100);
    //driveFor(14, 100);
    //pointAt(-10, -15, 100);
    //driveFor(28, 100);
    ////turnToHeading(0, 100);
    ////driveFor(10, 100);
    //turnToHeading(250, 100);
////
    //wingPneumatics.set(true);
    //
    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
    //wait(1200, msec);
    //leftDrive.stop();
    //rightDrive.stop();
    //wingPneumatics.set(false);
////
    //driveFor(24, 100);
    //turnToHeading(180, 100);
    //driveFor(-20, 100);
    //turnToHeading(270, 100);
    //wingPneumatics.set(true);
//
    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
    //wait(1000, msec);
//
    //wingPneumatics.set(false);
    //driveFor(28,100);
    //turnToHeading(180,100);
    //driveFor(-36, 100);
    //turnToHeading(300, 100);
//
    //// Double Push
//
    //wingPneumatics.set(true);
//
    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
    //wait(1000, msec);
    //leftDrive.stop();
    //rightDrive.stop();
//
    //driveFor(22, 100);
    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
//
    //wait(1000, msec);
    //driveFor(22, 100);
    //wingPneumatics.set(false);

   
}