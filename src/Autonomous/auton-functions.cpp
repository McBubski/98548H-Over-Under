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

void turnToHeading(double heading, double turnSpeed) {
    bool notDone = true;
    PID turnPid = PID(0.65, 0, 0, 2, 10, 100, &notDone, 20000, 200);

    double error = wrapAngleDeg(heading - Inertial.heading());
    double previousError = error;
    double previousTime = Brain.Timer.system();

    while (true) {
        double error = wrapAngleDeg(heading - Inertial.heading());
        double previousError = error;

        double speed = turnPid.Update(error, (Brain.Timer.system() - previousTime));

        leftDrive.spin(forward, speed * (turnSpeed / 100), percent);
        rightDrive.spin(reverse, speed * (turnSpeed / 100), percent);

        previousTime = Brain.Timer.system();

        wait(10, msec);
        Brain.Screen.printAt(50, 50, "%f", error);
    }

    leftDrive.stop();
    rightDrive.stop();
}

void pointAt(double x, double y, double turnSpeed) {
    targetOrientation = atan2(x - globalXPos, y - globalYPos);
    turnToHeading(targetOrientation * (180/M_PI), turnSpeed);
}

void driveFor(double distance, double speed) {
    double startHeading = Inertial.heading();
    double encoderStart = ForwardTrackingWheel.position(turns);

    targetX = globalXPos + sin(absoluteOrientation) * distance;
    targetY = globalYPos + cos(absoluteOrientation) * distance;

    bool notDone = true;
    bool rotationBool = true;
    PID drivePID = PID(6.5, 0.05, 0.02, 0.2, 10, 100, &notDone, 2000, 100);
    PID rotationCorrectionPID = PID(0.5, 0, 0, 5, 5, 5, &rotationBool, 2000, 100);

    double error = distance;
    double previousError = error;

    while (notDone) {
        double encoderChange = ForwardTrackingWheel.position(turns) - encoderStart;
        double inchesMoved = encoderChange * 2.75 * M_PI; // Circumference of Wheels

        error = distance - inchesMoved;
        previousError = error;

        double currentTargetOrientation = atan2(targetX - globalXPos,  targetY - globalYPos) * 180 / M_PI;

        double rotationDiff;
        if (error <= 2) {
            rotationDiff = wrapAngleDeg(startHeading - Inertial.heading(degrees));
        } else {
            rotationDiff = wrapAngleDeg(currentTargetOrientation - Inertial.heading(degrees));
        }
        double PIDOutput = drivePID.Update(error, 10);
        double rotationCorrection = (rotationDiff * 1);

        leftDrive.spin(forward, (PIDOutput + rotationCorrection) * (speed / 100), percent);
        rightDrive.spin(forward, (PIDOutput - rotationCorrection) * (speed / 100), percent);

        Brain.Screen.printAt(50, 50, "%f", rotationDiff);
        wait(10, msec);
    }

    leftDrive.stop();
    rightDrive.stop();
}