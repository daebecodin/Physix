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
