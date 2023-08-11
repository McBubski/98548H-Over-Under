#include "odometry.h"

// Constants

// Radius of tracking wheels in inches
const double WHEEL_RADIUS = 1.375;

// Starting angle in radians
const double THETA_START = 0.0;

// Starting Offset
double X_START_POS = -36.0;
double Y_START_POS = -60.0;

// Wheel Distance from Tracking Center, in INCHES
double ForwardTrackingDistance = 0.0;
double SideTrackingDistance = 5.625;

// Calculated Values
double FPos = 0;
double SPos = 0;

double FPosPrevious = 0;
double SPosPrevious = 0;

// Distance Travelled By Tracking Wheels Each Loop, in INCHES
double deltaDistF = 0;
double deltaDistS = 0;

// Total Sum Of Distances
double SumDeltaDistF = 0;

// Angle of robot (RADIANS);
double absoluteOrientation = THETA_START;
double previousTheta = THETA_START;
double deltaTheta = 0;

// Average Theta Throughout The Arc
double avgThetaThisArc = absoluteOrientation = (deltaTheta / 2);

// Change in X and Y position (INCHES)
double deltaXLocal = 0;
double deltaYLocal = 0;

double deltaXGlobal = 0;
double deltaYGlobal = 0;

double globalXPos = X_START_POS;
double globalYPos = Y_START_POS;

int positionTracking() {
    while (true) {
        // Gets tracking wheel position
        FPos = ForwardTrackingWheel.position(deg);
        SPos = SideTrackingWheel.position(deg);

        // Calculates change in sensor position (INCHES)
        deltaDistF = ((FPos - FPosPrevious) * M_PI / 180) * WHEEL_RADIUS;
        deltaDistS = ((SPos - SPosPrevious) * M_PI / 180) * WHEEL_RADIUS;

        // Update previous values (DEG)
        FPosPrevious = FPos;
        SPosPrevious = SPos;

        // Updates orientation (RAD)
        absoluteOrientation = ((Inertial.heading()) * (M_PI / 180.0));

        deltaTheta = absoluteOrientation - previousTheta;
        previousTheta = absoluteOrientation;

        if (deltaTheta == 0) {
            deltaXLocal = deltaDistS;
            deltaYLocal = deltaDistF;
        } else {
            deltaXLocal = 2 * sin(deltaTheta / 2.0) * ((deltaDistS / deltaTheta) + SideTrackingDistance);
            deltaYLocal = 2 * sin(deltaTheta / 2.0) * ((deltaDistF / deltaTheta) + ForwardTrackingDistance);
        }

        avgThetaThisArc = absoluteOrientation - (deltaTheta / 2);

        deltaXGlobal = (deltaYLocal * cos(avgThetaThisArc)) - (deltaXLocal * sin(avgThetaThisArc));
        deltaYGlobal = (deltaYLocal * sin(avgThetaThisArc)) + (deltaXLocal * cos(avgThetaThisArc));

        while(absoluteOrientation >= 2 * M_PI) {
            absoluteOrientation -= 2 * M_PI;
        }
    
        while(absoluteOrientation < 0) {
            absoluteOrientation += 2 * M_PI;
        }

        globalXPos += deltaXGlobal;
        globalYPos += deltaYGlobal;

        task::sleep(10);
    }
    return 0;
}