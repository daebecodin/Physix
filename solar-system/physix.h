#pragma once

struct Vec3 {
    double x {};
    double y {};
    double z {};
};

struct Body {
    double mass {}; // kg
    Vec3 position {}; // m
    Vec3 velocity {}; // m/s
};

Vec3 getGravitationlAcceleration(const Body& attrcter, const Body& attractod);
Vec3 getSpeed(double vx, double vy, double vz);
Vec3 resolveComponents(double accelerationMagnitude, double dx, double dy, double dz);
double getDistanceMagnitude(double dx, double dy, double dz);
double getAccelerationMagnitude(double atractorMass, double distance);
double getCircularOrbitSpeed(double attracterMass, double distance);
double getDelta(double d2, double d1);
