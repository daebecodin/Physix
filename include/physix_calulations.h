#pragma once
#include "vec3.h"
#include "body.h"
#include <vector>
#include <cstddef>

namespace Physix::Orbital {

    Vec3 gravitationlAcceleration(const Body& attrcter, const Body& attractod);

    Vec3 vectorComponents(double accelerationMagnitude, double dx, double dy, double dz);

    double distanceMagnitude(double dx, double dy, double dz);

    double accelerationMagnitude(double atractorMass, double distance);

    double circularOrbitSpeed(double attracterMass, double distance);

    double delta(double d2, double d1);

    std::vector<Vec3> systemAccelerations(const std::vector<Body>& bodies);

    double distanceFromSun(const std::vector<Body>& bodies, std::size_t bodyIndex);

}


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
