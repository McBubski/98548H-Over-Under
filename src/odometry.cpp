#include "odometry.h"
#include "vex.h"

float absoluteOrientation = 0.0;

int positionTracking() {
    while (true) {
        absoluteOrientation = Inertial.heading();
    }
    return 0;
}