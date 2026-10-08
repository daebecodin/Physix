#pragma once
#include "vec3.h"
#include "body.h"
#include <vector>

namespace Physix::Calculate {

    namespace Orbital {

        Vec3 gravitationlAcceleration(const Body& attrcter, const Body& attractod);

        Vec3 speed(double vx, double vy, double vz);
        Vec3 vectorComponents(double accelerationMagnitude, double dx, double dy, double dz);

        double distanceMagnitude(double dx, double dy, double dz);

        double accelerationMagnitude(double atractorMass, double distance);

        double circularOrbitSpeed(double attracterMass, double distance);

        double delta(double d2, double d1);

        std::vector<Vec3> getSystemAccelerations(const std::vector<Body>& bodies);

        double distanceFromSun(const std::vector<Body>& bodies, std::size_t bodyIndex);

    }

    namespace Projectile {
        double setDegreesToRadians(double theta);

        std::vector<double> solveQuadratic(double a, double b, double c);

        double getTimeOfFlight(double verticalComponent, double yInitial, double yFinal);

        double resolveComponents(double magnitude, double theta);

        double getHorizontalDiaplacement(double horizontalComponent, double timeOfFlight);

        double getLandingVelocity(double initialVx, double initialVy, double timeOfFlight);

        double getSpeed(double vX, double vY);

        double getMaximumHeight(double initialVy, double launchGeight);

    }
}
