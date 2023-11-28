#include "Autonomous/pure-pursuit.h"
#include "Autonomous/odometry.h"
#include "Autonomous/PID.h"
#include "Autonomous/auton-functions.h"
#include <vector>
#include <numeric>
#include <sstream>
#include "Graphics/graphics.h"

#include <vex.h>

Waypoint::Waypoint() : X(0.0), Y(0.0), TargetVelocity(0.0), Curvature(0.0), DistanceAlongPath(0.0) {

}
Waypoint::Waypoint(double x, double y, double targetVelocity, double curvature, double distanceAlongPath)
    : X(x), Y(y), TargetVelocity(targetVelocity), Curvature(curvature), DistanceAlongPath(distanceAlongPath) {
}

float wrapAngleDeg(float angle) {
    angle = fmod(angle + 180.0, 360.0);
    return (angle >= 0) ? angle - 180.0 : angle + 180.0;
}

std::vector<float> getPointsFromFile(const char *fileName) {
    char myData[15000];
    Brain.SDcard.loadfile(fileName, (uint8_t *)myData, sizeof(myData));

    std::stringstream ss(myData);
    std::vector<float> floatArray;

    float number;

    while (ss >> number) {
        floatArray.push_back(number);
    }

    return floatArray;
}

int findClosestPoint(const Waypoint* path, int indexOfClosestPoint, int numOfPoints) {
    int closestIndex = 0;
    float previousClosestDistance = 999999;

    for (int i = indexOfClosestPoint; i < numOfPoints; i++) {
        float a = globalXPos - path[i].X;
        float b = globalYPos - path[i].Y;
        float dist = sqrt(a * a + b * b);

        if (dist < previousClosestDistance) {
            previousClosestDistance = dist;
            closestIndex = i;
        }
    }

    return closestIndex;
}

float getDistance(float x1, float y1, float x2, float y2) {
    float a = x1 - x2;
    float b = y1 - y2;
    float dist = sqrt(a * a + b * b);

    return dist;
}

float previousHighestFractionalIndex = 0;

std::vector<double> findLookaheadPoint(Waypoint Points[], int numPoints, double lookaheadDistance) {
    std::vector<double> intersectionPoint;

    for (int lineSegment = 0; lineSegment < numPoints - 1; lineSegment++) {
        Waypoint PointE = Points[lineSegment];
        std::vector<double> E {PointE.X, PointE.Y};

        Waypoint PointL = Points[lineSegment + 1];
        std::vector<double> L {PointL.X, PointL.Y};

        std::vector<double> C {globalXPos, globalYPos};
        std::vector<double> d {L[0] - E[0], L[1] - E[1]};
        std::vector<double> f {E[0] - C[0], E[1] - C[1]};

        double a = inner_product(d.begin(), d.end(), d.begin(), 0);
        double b = 2 * inner_product(f.begin(), f.end(), d.begin(), 0);
        double c = inner_product(f.begin(), f.end(), f.begin(), 0) - lookaheadDistance * lookaheadDistance;

        double discriminant = b*b-4*a*c;

        double t = 0;

        if (discriminant < 0) {

        } else {
            double t1 = (-b - discriminant) / (2 * a);
            double t2 = (-b + discriminant) / (2 * a);

            if (t1 >= 0 && t1 <= 1) {
                t = t1;
            }
            if (t2 >= 0 && t2 <= 1) {
                t = t2;
            }
        }

        float fractionalIndex = t + lineSegment;

        if (fractionalIndex > previousHighestFractionalIndex) {
            intersectionPoint.push_back(E[0] + t * d[0]);
            intersectionPoint.push_back(E[1] + t * d[1]);
        }
    }

    return intersectionPoint;
}

void drawPoints(Waypoint Points[], int numPoints, int closestPoint) {
    float XOnBrainScreen = 360 + (1.39 * globalXPos);
    float YOnbrainScreen = 120 + (-1.39 * globalYPos);
    
    Brain.Screen.drawCircle(XOnBrainScreen, YOnbrainScreen, 36 * 1.39);
}

void followPath(const char *fileName) {
    
    std::vector<float> coordinates = getPointsFromFile(fileName); // Gets Points From File
    int numPoints = (coordinates.size() / 5);

    Waypoint waypoints[numPoints]; // Makes Array for Waypoints Class

    for (int i = 0; i < numPoints; i++) {
        int pointIndex = i * 5;

        double x = coordinates[pointIndex];
        double y = coordinates[pointIndex + 1];
        double targetVelocity = coordinates[pointIndex + 2];
        double curvature = coordinates[pointIndex + 3];
        double distanceAlongPath = coordinates[pointIndex + 4];

        waypoints[i] = Waypoint(x, y, targetVelocity, curvature, distanceAlongPath); // Makes Waypoints based on Coordinates
    }

    // CONTROL ALGORITHM

    int closestPointIndex = 0;
    std::vector<double> lookaheadPoint;

    while (true) {
        closestPointIndex = findClosestPoint(waypoints, closestPointIndex, numPoints);

        lookaheadPoint = findLookaheadPoint(waypoints, numPoints, 36);

        printf("%f, %f\n", lookaheadPoint[0], lookaheadPoint[1]);
        
        drawPoints(waypoints, numPoints, closestPointIndex);
        wait(20, msec);
    }
};