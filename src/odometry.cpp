#include "odometry.h"

// Constants

// Radius of tracking wheels in inches
const double WHEEL_RADIUS = 1.375;

double absoluteOrientation = 0;

double globalXPos = -36.0;
double globalYPos = -60.0;

// Calculated Values
double wheelPos = 0;
double wheelPosPrevious = 0;

double deltaDistance = 0;

int positionTracking() {
    while (true) {
        wheelPos = ForwardTrackingWheel.position(deg);
        deltaDistance = ((wheelPos - wheelPosPrevious) * M_PI / 180) * WHEEL_RADIUS;
        wheelPosPrevious = wheelPos;

        absoluteOrientation = ((Inertial.heading()) * (M_PI / 180.0));

        globalXPos += sin(absoluteOrientation) * deltaDistance;
        globalYPos += cos(absoluteOrientation) * deltaDistance;

        task::sleep(10);
    }
    return 0;
}