#include "vex.h"
#include "Autonomous/autonomous.h"
#include "Autonomous/odometry.h"
#include "Autonomous/auton-functions.h"
#include "Autonomous/PID.h"

int autonPath = 1;

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
<<<<<<< HEAD
    driveFor(48, 100);
    //driveFor(-22, 100);
    //turnToHeading(180, 100); // Align with Goal
//
    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
//
    //wait(500, msec);
//
    //leftDrive.stop();
    //rightDrive.stop();
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
    driveFor(-32, 100);
=======
    driveFor(-22, 100);
    turnToHeading(180, 100); // Align with Goal

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);

    wait(500, msec);

    leftDrive.stop();
    rightDrive.stop();

    driveFor(4, 100);

    pointAt(-46, -46, 100); // Align To Matchload
    driveFor(16, 100);

    turnToHeading(45, 100);
    leftDrive.spin(reverse, 15, percent);
    rightDrive.spin(reverse, 15, percent);
    wait(1500, msec);
    leftDrive.stop();
    rightDrive.stop();

    triball_arm.spinTo(140, degrees, true);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
    driveFor(10, 100);
    //turnToHeading(0, 100); // Align with Goal

<<<<<<< HEAD
    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
=======
    triball_arm.spinTo(25, degrees, false);
    turnToHeading(135, 100);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4

    //wait(500, msec);

<<<<<<< HEAD
    //leftDrive.stop();
    //rightDrive.stop();

    //driveFor(4, 100);

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
=======
    turnToHeading(90, 100);
    lift.spinTo(390, degrees, false);
    driveFor(18, 50);
}

void awayAuton() {
   
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
}

void skills() {
    driveFor(-6, 100);
    turnToHeading(240, 100);
<<<<<<< HEAD
    driveFor(1, 100);
//
    puncher.spin(forward, 100, percent);
    wait(30, seconds);
=======
    driveFor(3, 100);
//
    puncher.spin(forward, 75, percent);
    wait(10, seconds);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
    puncher.stop();
//
    driveFor(-4, 100);
    turnToHeading(305, 100);
//
    driveFor(-10, 100);
<<<<<<< HEAD
    turnToHeading(270, 100);
=======
    turnToHeading(265, 100);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
//
    driveFor(-72, 100);
    turnToHeading(0, 100);
    driveFor(14, 100);
    pointAt(-12, -20, 100);
<<<<<<< HEAD
    driveFor(28, 100);
    //turnToHeading(0, 100);
    //driveFor(10, 100);
    turnToHeading(250, 100);
//
    wingPneumatics.set(true);
=======
    driveFor(32, 100);
    turnToHeading(0, 100);
    driveFor(24, 100);
    turnToHeading(265, 100);
//
    elevationPneumatics.set(true);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
    
    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(1000, msec);
    leftDrive.stop();
    rightDrive.stop();
<<<<<<< HEAD
    wingPneumatics.set(false);
//
    driveFor(20, 100);
    turnToHeading(180, 100);
    driveFor(-8, 100);
    turnToHeading(270, 100);
    wingPneumatics.set(true);

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(1000, msec);

    wingPneumatics.set(false);
    driveFor(28,100);
    turnToHeading(180,100);
    driveFor(-21, 100);
    turnToHeading(300, 100);
    wingPneumatics.set(true);
=======
//
    driveFor(12, 100);
    elevationPneumatics.set(false);

    pointAt(48, -48, 100);
    driveFor(40, 100);
    turnToHeading(115, 100);

    driveFor(18, 100);
    turnToHeading(225, 100);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);
    wait(1000, msec);
    leftDrive.stop();
    rightDrive.stop();

<<<<<<< HEAD
    driveFor(22, 100);
    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);

    wait(1000, msec);
    driveFor(22, 100);
    wingPneumatics.set(false);

    //pointAt(48, -48, 100);
    //driveFor(40, 100);
    //turnToHeading(115, 100);

    //driveFor(18, 100);
    //turnToHeading(225, 100);

    //leftDrive.spin(reverse, 100, percent);
    //rightDrive.spin(reverse, 100, percent);
    //wait(1000, msec);
    //leftDrive.stop();
    //rightDrive.stop();

    //driveFor(12, 100);




=======
    driveFor(12, 100);
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
// OLD!


//
    //driveFor(-52, 100);
<<<<<<< HEAD
    //wait(3600, msec);
    //turnTo(-45, 80);
//
    //drive.driveFor(-10, inches, false);
    //wait(1000, msec);
    //turnTo(80, 80);
//
    //drive.driveFor(56, inches, false);
    //wait(1000, msec);
    //turnTo(-130, 80);
//
    //drive.driveFor(-20, inches, false);
    //wait(2000, msec);
    //turnTo(90, 80);
//
    //elevationPneumatics.set(true);
//
    //drive.setDriveVelocity (-80, percent);
    //drive.driveFor(-36, inches, false);
    //wait(1500, msec);
//
    //elevationPneumatics.set(false);
    //drive.driveFor(6, inches, true);
//
    //driveFor(-52, 100);
=======
>>>>>>> 768b2ac8ffd01417197e51bc9c9339eb80077ff4
    //wait(3600, msec);
    //turnTo(-45, 80);
//
    //drive.driveFor(-10, inches, false);
    //wait(1000, msec);
    //turnTo(80, 80);
//
    //drive.driveFor(56, inches, false);
    //wait(1000, msec);
    //turnTo(-130, 80);
//
    //drive.driveFor(-20, inches, false);
    //wait(2000, msec);
    //turnTo(90, 80);
//
    //elevationPneumatics.set(true);
//
    //drive.setDriveVelocity (-80, percent);
    //drive.driveFor(-36, inches, false);
    //wait(1500, msec);
//
    //elevationPneumatics.set(false);
    //drive.driveFor(6, inches, true);
}