#include <vector>

class Waypoint {
public:
    double X;
    double Y;
    double TargetVelocity;
    double Curvature;
    double DistanceAlongPath;

    Waypoint();  // Default constructor
    Waypoint(double x, double y, double targetVelocity, double curvature, double distanceAlongPath);  // Parameterized constructor

    // Add other member functions if needed
};

void followPath(const char *fileName);