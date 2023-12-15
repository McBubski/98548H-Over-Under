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

double getDistance(double x1, double y1, double x2, double y2) {
    double a = (x1 - x2) * (x1 - x2);
    double b = (y1 - y2) * (y1 - y2);
    double dist = sqrt(a + b);

    return dist;
}

void turnToHeading(double heading, double turnSpeed) {
    bool notDone = true;
    PID turnPid = PID(0.7815, 0.00635, 3.97, 2, 3, 100, &notDone, 600, 100);
    
    double error = wrapAngleDeg(heading - Inertial.heading());
    double previousError = error;
    double previousTime = Brain.Timer.system();

    while (notDone) {
        double error = wrapAngleDeg(heading - Inertial.heading());
        double previousError = error;

        double speed = turnPid.Update(error, (Brain.Timer.system() - previousTime));

        leftDrive.spin(forward, speed * (turnSpeed / 100), percent);
        rightDrive.spin(reverse, speed * (turnSpeed / 100), percent);

        previousTime = Brain.Timer.system();

        wait(10, msec);
    }

    leftDrive.stop();
    rightDrive.stop();
}

void pointAt(double x, double y, double turnSpeed, vex::directionType dir) {
    targetOrientation = atan2(x - globalXPos, y - globalYPos);
    if (dir == forward) {
        turnToHeading(targetOrientation * (180/M_PI), turnSpeed);
    } else if (dir == reverse) {
        turnToHeading(targetOrientation * (180/M_PI) + 180, turnSpeed);
    }
}

void driveFor(double distance, double speed) {
    double targetHeading = Inertial.heading();
    double encoderStart = ForwardTrackingWheel.position(turns);


    bool driving = true;
    bool turning = true;
    PID drivePID = PID(8, 0.01, 24.105, 0.2, 10, 100, &driving, 2000, 150);
    PID turnPID = PID(0.7815, 0, 3.97, 2, 8, 100, &turning, 2000, 100);

    double driveError = distance;
    double turnError = wrapAngleDeg(targetHeading - Inertial.heading());

    double previousTime = Brain.Timer.system();

    while (driving) {
        double encoderChange = ForwardTrackingWheel.position(turns) - encoderStart;
        double inchesMoved = encoderChange * 2.75 * M_PI; // Circumference of Wheels

        driveError = distance - inchesMoved;

        turnError = wrapAngleDeg(targetHeading - Inertial.heading());

        double dt = (Brain.Timer.system() - previousTime);

        double driveOutput = drivePID.Update(driveError, dt);
        double turnOutput = turnPID.Update(turnError, dt);

        leftDrive.spin(forward, (driveOutput + turnOutput) * (speed / 100), percent);
        rightDrive.spin(forward, (driveOutput - turnOutput) * (speed / 100), percent);

        previousTime = Brain.Timer.system();

        wait(10, msec);

        Brain.Screen.printAt(50, 50, "%f", driveError);
    }

    leftDrive.stop();
    rightDrive.stop();
}

void driveTo(double x, double y, double speed, vex::directionType dir) {
    pointAt(x, y, 100, dir);

    if (dir == forward) {
        double dist = getDistance(globalXPos, globalYPos, x, y);
        driveFor(dist, speed);
    }

    if (dir == reverse) {
        double dist = getDistance(globalXPos, globalYPos, x, y);
        driveFor(-dist, speed);
    }
}