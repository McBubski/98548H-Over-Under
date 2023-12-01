#include "Autonomous/pure-pursuit.h"
#include "Autonomous/odometry.h"
#include "Autonomous/PID.h"
#include "Autonomous/auton-functions.h"
#include <vector>
#include <numeric>
#include <sstream>
#include "Graphics/graphics.h"

#include <vex.h>

const double Kv = 0.1;
const double Ka = 0.002;
const double Kp = 0.01;
const double maxChange = 10;

double lastLeftOutput = 0.0;
double lastRightOutput = 0.0;
double prevTargetVelocityLeft = 0.0;
double prevTargetVelocityRight = 0.0;

double leftWheelSpeed = 0.0;
double rightWheelSpeed = 0.0;

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

    return intersectionPoint;
}

std::vector<double> calculateLookaheadPoint(Waypoint E, Waypoint L, std::vector<double> C, double lookaheadDistance) {
    std::vector<double> d {L.X - E.X, L.Y - E.Y};
    std::vector<double> f = {E.X - C[0], E.Y - C[1]};

    double a = (d[0] * d[0] + d[1] * d[1]);
    double b = 2 * (f[0] * d[0] + f[1] * d[1]);
    double c = f[0] * f[0] + f[1] * f[1] - lookaheadDistance * lookaheadDistance;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::vector<double> {-1, -1};
    } else {
        discriminant = sqrt(discriminant);
        double t1 = (-b - discriminant) / (2 * a);
        double t2 = (-b + discriminant) / (2 * a);

        if (t1 >= 0 && t1 <= 1) {
            return std::vector<double> {E.X + t1 * d[0], E.Y + t1 * d[1]};
        }

        if (t2 >= 0 && t2 <= 1) {
            return std::vector<double> {E.X + t2 * d[0], E.Y + t2 * d[1]};
        }

        return std::vector<double> {-1, -1};
    }
}

std::pair<std::vector<double>, int> findLookaheadAlongPath(Waypoint path[], int numPoints, double lookaheadDistance, int lastLookaheadIndex) {
    std::vector<double> lookaheadPoint {-1, -1};
    std::vector<double> robotPos {globalXPos, globalYPos};

    for (int i = lastLookaheadIndex; i < numPoints - 1; i++) {
        Waypoint startPoint = path[i];
        Waypoint endPoint = path[i + 1];

        std::vector<double> intersection = calculateLookaheadPoint(startPoint, endPoint, robotPos, lookaheadDistance);

        if (intersection[0] != -1 && intersection[1] != -1) {
            double t = sqrt(pow(intersection[0] - startPoint.X, 2) + pow(intersection[1] - startPoint.Y, 2)) /
                        sqrt(pow(endPoint.X - startPoint.X, 2) + pow(endPoint.Y - startPoint.Y, 2));

            if (t >= 0 && t <= 1) {
                int fractionalIndex = i + t;

                if (fractionalIndex > lastLookaheadIndex) {
                    lookaheadPoint = intersection;
                    lastLookaheadIndex = fractionalIndex;
                    break;
                }
            }
        }
    }
    
    if (lookaheadPoint[0] == -1 && lookaheadPoint[1] == -1) {
        lookaheadPoint.clear();

        lookaheadPoint.push_back(path[lastLookaheadIndex].X);
        lookaheadPoint.push_back(path[lastLookaheadIndex].Y);
    }

    return std::make_pair(lookaheadPoint, lastLookaheadIndex);
}

void drawPoints(Waypoint Points[], int numPoints, int closestPoint) {
    float XOnBrainScreen = 360 + (1.39 * globalXPos);
    float YOnbrainScreen = 120 + (-1.39 * globalYPos);
    
    Brain.Screen.drawCircle(XOnBrainScreen, YOnbrainScreen, 36 * 1.39);
}

double calculateSignedCurvature(double lookahead_distance, std::vector<double> lookaheadPoint) {
    double a = -tan(absoluteOrientation);
    double b = 1;
    double c = tan(absoluteOrientation) * globalXPos - globalYPos;
    double x = std::abs(a * lookaheadPoint[0] + b * lookaheadPoint[1] + c) / sqrt(a * a + b * b);

    double bX = globalXPos + cos(absoluteOrientation);
    double bY = globalYPos + sin(absoluteOrientation);

    double crossProduct = (bY - globalYPos) * (lookaheadPoint[0] - globalXPos) - (bX - globalXPos) * (lookaheadPoint[1] - globalYPos);
    int side = (crossProduct > 0) ? 1 : -1;

    double curvature = 2 * x / (lookahead_distance * lookahead_distance);
    return curvature * side;
}

double rateLimiter(double input, double lastOutput, double maxChange, double deltaTime) {
    double output = lastOutput + std::copysign(std::min(std::abs(input - lastOutput), maxChange), input - lastOutput);
    return output;
}

void calculateWheelSpeeds(double targetVelocity, double curvature, double trackWidth, double& leftSpeed, double& rightSpeed) {
    double calculatedLeft = targetVelocity * (2 + curvature * trackWidth) / 2;
    double calculatedRight = targetVelocity * (2 - curvature * trackWidth) / 2;
    leftSpeed = calculatedLeft;
    rightSpeed = calculatedRight;
}

void controlWheelVelocities(double targetVelocity, double trackWidth, double curvature, double dT) {
    double leftSpeed, rightSpeed;

    calculateWheelSpeeds(targetVelocity, curvature, trackWidth, leftSpeed, rightSpeed);

    double targetAccelerationLeft = (leftSpeed - prevTargetVelocityLeft) / dT;
    double targetAccelerationRight = (rightSpeed - prevTargetVelocityRight) / dT;

    prevTargetVelocityLeft = leftSpeed;
    prevTargetVelocityRight = rightSpeed;

    double rateLimitedLeft = rateLimiter(leftSpeed, lastLeftOutput, maxChange, dT);
    double rateLimitedRight = rateLimiter(rightSpeed, lastRightOutput, maxChange, dT);

    double leftFF = Kv * rateLimitedLeft + Ka * targetAccelerationLeft;
    double rightFF = Kv * rateLimitedRight + Ka * targetAccelerationRight;

    double measuredLeftVelocity = leftDrive.velocity(pct);
    double measuredRightVelocity = rightDrive.velocity(pct);

    double leftFB = Kp * (rateLimitedLeft - measuredLeftVelocity);
    double rightFB = Kp * (rateLimitedRight - measuredRightVelocity);

    double leftPower = leftFF + leftFB;
    double rightPower = rightFF + rightFB;

    Brain.Screen.printAt(150, 150, "%f, %f", leftPower, rightPower);
    
    lastLeftOutput = rateLimitedLeft;
    lastRightOutput = rateLimitedRight;

    //leftDrive.spin(forward, leftPower, percent);
    //rightDrive.spin(forward, rightPower, percent);
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
    double lookaheadDistance = 12;
    int lastLookaheadIndex = 0;
    std::vector<double> lookaheadPoint;

    double trackWidth = 14;

    double previousTime = Brain.Timer.systemHighResolution();

    while (true) {
        closestPointIndex = findClosestPoint(waypoints, closestPointIndex, numPoints);

        std::pair<std::vector<double>, int> lookaheadInfo = findLookaheadAlongPath(waypoints, numPoints, lookaheadDistance, lastLookaheadIndex);
        lookaheadPoint = lookaheadInfo.first;
        lastLookaheadIndex = lookaheadInfo.second;

        double targetVelocity = waypoints[closestPointIndex].TargetVelocity;
        double signedCurvature = calculateSignedCurvature(lookaheadDistance, lookaheadPoint);

        double leftSpeed = 0;
        double rightSpeed = 0;

        double dT = (Brain.Timer.systemHighResolution() - previousTime);
        previousTime = Brain.Timer.systemHighResolution();

        controlWheelVelocities(targetVelocity, trackWidth, signedCurvature, dT);

        leftDrive.spin(forward, leftWheelSpeed, percent);
        rightDrive.spin(forward, rightWheelSpeed, percent);



        //Brain.Screen.printAt(150, 150, "%f, %f", lookaheadDistance, absoluteOrientation);

        //float XOnBrainScreen = 360 + (1.39 *  lookaheadPoint[0]);
        //float YOnbrainScreen = 120 + (-1.39 * lookaheadPoint[1]);
//
        //float robotX = 360 + (1.39 * globalXPos);
        //float robotY = 120 + (-1.39 * globalYPos);
//
        //Brain.Screen.drawCircle(XOnBrainScreen, YOnbrainScreen, 5  );
        //Brain.Screen.drawCircle(robotX, robotY, 1.39 * lookaheadDistance);

        //drawPoints(waypoints, numPoints, closestPointIndex);
        wait(20, msec);
    }
};