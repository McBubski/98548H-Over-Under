#include "vex.h"
#include "Autonomous/PID.h"
#include "Autonomous/odometry.h"

double targetX = 0;
double targetY = 0;

double localXError = 0;
double localYError = 0;

double targetOrientation = 0;

double wrapAngleDeg(double angle) {
    angle = fmod(angle + 180.0, 360.0);
    return (angle >= 0) ? angle - 180.0 : angle + 180.0;
}

void turnToHeading(double heading) {
    bool notDone = true;
    PID turnPid = PID(0.6, 0.004, 0.1, 2, 10, 100, &notDone, 2000, 2000);

    double error = wrapAngleDeg(heading - Inertial.heading());
    double previousError = error;

    while (notDone) {
        double error = wrapAngleDeg(heading - Inertial.heading());
        double previousError = error;

        double speed = turnPid.Update(error, 10);

        leftDrive.spin(forward, speed, percent);
        rightDrive.spin(reverse, speed, percent);
        wait(10, msec);
    }

    leftDrive.stop();
    rightDrive.stop();
}

void pointAt(double x, double y) {
    targetOrientation = atan2(x - globalXPos, y - globalYPos);
    turnToHeading(targetOrientation * (180/M_PI));
}

void driveFor(double distance, distanceUnits unit) {
    double encoderStart = ForwardTrackingWheel.position(turns);

    targetX = globalXPos + sin(absoluteOrientation) * distance;
    targetY = globalYPos + cos(absoluteOrientation) * distance;

    bool notDone = true;
    bool rotationBool = true;
    PID drivePID = PID(6, 0.05, 0.02, 0.2, 10, 100, &notDone, 500000000, 500);
    PID rotationCorrectionPID = PID(0.5, 0, 0, 5, 10, 5, &rotationBool, 5000000,500);

    double error = distance;
    double previousError = error;

    while (notDone) {
        double encoderChange = ForwardTrackingWheel.position(turns) - encoderStart;
        double inchesMoved = encoderChange * 2.75 * M_PI; // Circumference of Wheels

        error = distance - inchesMoved;
        previousError = error;

        double currentTargetOrientation = atan2(targetX - globalXPos,  targetY - globalYPos) * 180 / M_PI;

        double rotationDiff = wrapAngleDeg(currentTargetOrientation - Inertial.heading(degrees));

        double PIDOutput = drivePID.Update(error, 10);
        double rotationCorrection = (rotationDiff * 0.7);
        if (error <= 0.5) {
            rotationCorrection = 0;
        }

        leftDrive.spin(forward, (PIDOutput + rotationCorrection) * 0.5, percent);
        rightDrive.spin(forward, (PIDOutput - rotationCorrection) * 0.5, percent);

        Brain.Screen.printAt(50, 50, "%f", rotationDiff);
        wait(10, msec);
    }

    leftDrive.stop();
    rightDrive.stop();
}