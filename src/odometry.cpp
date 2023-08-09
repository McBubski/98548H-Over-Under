#include "odometry.h"
#include "vex.h"

float absoluteOrientation = 0.0;
float globalXPos = -24.0;
float globalYPos = 0.0;

int positionTracking() {
    while (true) {
        absoluteOrientation = (Inertial.heading() * (M_PI/ 180));
    }
    return 0;
}