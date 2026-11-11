#pragma once

#include <vector>

namespace Physix::Projectile {
    double degreesToRadians(double theta);

    std::vector<double> solveQuadratic(double a, double b, double c);

    double timeOfFlight(double verticalComponent, double yInitial, double yFinal);

    std::vector<double> vectorComponents(double magnitude, double theta);

    double horizontalDisplacement(double horizontalComponent, double timeOfFlight);

    std::vector<double> landingVelocity(double initialVx, double initialVy, double timeOfFlight);

    double maximumHeight(double initialVy, double launchGeight);

    double velocityMagnitude(double vx, double vy);
}
