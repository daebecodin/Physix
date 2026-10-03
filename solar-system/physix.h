#pragma once

struct Vec3 {
    double x {};
    double y {};
    double z {};

    /*
     * we apply the change to the lhs body's compnents
     */
    Vec3& operator+= (const Vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    /*
     * scalar multiplication
     * we leave the lhs unchanged 
     * creates a temporary vector to be added to bodies vector in compound assignment
     */
    Vec3 operator* (double scalar) const
    {
        return {
            x * scalar,
            y * scalar,
            z * scalar
        };
    }



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
std::vector<Vec3> getSystemAccelerations(const std::vector<Body>& bodies);
void simulateSystem(std::vector<Body>& bodies, double dt);

