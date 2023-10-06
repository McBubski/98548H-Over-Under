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
    driveFor(-22, 100);
    turnToHeading(180, 100);

    driveFor(-10, 100);
    driveFor(2, 100);

    pointAt(-45, -49, 100); // Align To Matchload
    driveFor(11, 100);

    turnToHeading(45, 100);
    driveFor(-5.5, 50); // Pickup Triball

    triball_arm.spinTo(140, degrees, true);
    driveFor(10, 100);

    triball_arm.spinTo(0, degrees, false);
    turnToHeading(135, 50);

    driveFor(20, 100);

    turnToHeading(90, 100);
    triball_arm.spinTo(25, degrees, false);
    driveFor(16, 80);

    lift.spinTo(650, degrees);
}

void skills() {
    turnToHeading(225, 100);
    //driveFor(12, 100);
    //puncher.spin(forward, 75, percent);
    //wait(40, seconds);
    //puncher.stop();
    //drive.setDriveVelocity(55, percent);
//
    //turnTo(25, 80);
//
    //drive.driveFor(-54, inches, false); // Crosses Line
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