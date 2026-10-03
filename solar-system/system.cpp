#include "pch.h"
#include "physix.h"
#include <stdexcept>

constexpr double UNIVERSAL_GRAVITY = 6.67430e-11;
constexpr double SUN_MASS = 1.9884e30; // kg
// Planet masses in kilograms
constexpr double MERCURY_MASS = 3.30e23;
constexpr double VENUS_MASS   = 4.87e24;
constexpr double EARTH_MASS   = 5.97e24;
constexpr double MARS_MASS    = 6.42e23;
constexpr double JUPITER_MASS = 1.898e27;
constexpr double SATURN_MASS  = 5.68e26;
constexpr double URANUS_MASS  = 8.68e25;
constexpr double NEPTUNE_MASS = 1.02e26;
// Approximate orbital semi-major axes, meters
constexpr double MERCURY_ORBITAL_DISTANCE = 5.79e10;
constexpr double VENUS_ORBITAL_DISTANCE   = 1.082e11;
constexpr double EARTH_ORBITAL_DISTANCE   = 1.496e11;
constexpr double MARS_ORBITAL_DISTANCE    = 2.280e11;
constexpr double JUPITER_ORBITAL_DISTANCE = 7.785e11;
constexpr double SATURN_ORBITAL_DISTANCE  = 1.432e12;
constexpr double URANUS_ORBITAL_DISTANCE  = 2.867e12;
constexpr double NEPTUNE_ORBITAL_DISTANCE = 4.515e12;

double getDelta(double d2, double d1)
{
    return d2 - d1;
}

double getDistanceMagnitude(double dx, double dy, double dz)
{
    return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

double getAccelerationMagnitude(double attracterMass, double distance)
{
    return (UNIVERSAL_GRAVITY * attracterMass) / (distance * distance);
}

double getCircularOrbitSpeed(double attrcterMass, double distance)
{
    return std::sqrt((UNIVERSAL_GRAVITY * attrcterMass) / distance);
}

Vec3 resolveComponents(double accelerationMagnitude, double dx, double dy, double dz)
{
    double r = getDistanceMagnitude(dx, dy, dz);

    if (r == 0) 
    {
        throw std::invalid_argument("Bodies must have non-zero separations");
    }

    return 
    {
        accelerationMagnitude * dx / r,
        accelerationMagnitude * dy / r,
        accelerationMagnitude * dz / r
    };

}

Vec3 getGravitationlAcceleration(const Body& attracter, const Body& attracted){
    
    double dx = getDelta(attracter.position.x, attracted.position.x);
    double dy = getDelta(attracter.position.y, attracted.position.y);
    double dz = getDelta(attracter.position.z, attracted.position.z);


    double r = getDistanceMagnitude(dx, dy, dz);

    if (r == 0) 
    {
        throw std::invalid_argument("Bodies must have non-zero separations");
    }

    double a = getAccelerationMagnitude(attracter.mass, r);

    return resolveComponents(a, dx, dy, dz);
}


int main() 
{
    Body sun {SUN_MASS, {0.0, 0.0, 0.0}, {}};
    Body mercury {MERCURY_MASS, {MERCURY_ORBITAL_DISTANCE, 0.0, 0.0}, {}};

    std::cout << "---Mercury---\n";
    Vec3 mercuryAccelerationFromSun = getGravitationlAcceleration(sun, mercury);
    std::cout << "Accleration Components -> "
                << mercuryAccelerationFromSun.x << ", " 
                << mercuryAccelerationFromSun.y << ", "
                << mercuryAccelerationFromSun.z << '\n';

    double mercuryOrbitSpeed = getCircularOrbitSpeed(sun.mass, MERCURY_ORBITAL_DISTANCE);
    mercury.velocity = {0.0, mercuryOrbitSpeed, 0.0};
    std::cout << "Orbit Speed -> " << mercuryOrbitSpeed << " m/s\n";


    return 0;

} 
