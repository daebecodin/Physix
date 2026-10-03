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

/*
 * Calculates the gravitation acceleration between two bodies
 */
Vec3 getGravitationlAcceleration(const Body& attracter, const Body& attracted){
    
    double dx = getDelta(attracter.position.x, attracted.position.x);
    double dy = getDelta(attracter.position.y, attracted.position.y);
    double dz = getDelta(attracter.position.z, attracted.position.z);


    // magnitude of the difference of between centers
    double r = getDistanceMagnitude(dx, dy, dz);

    if (r == 0) 
    {
        throw std::invalid_argument("Bodies must have non-zero separations");
    }


    double a = getAccelerationMagnitude(attracter.mass, r);

    return resolveComponents(a, dx, dy, dz);
}

/*
 * sums the gravitational acceleration vectors influenced by each surrounding body for each body,
 * excluding itself
 */
std::vector<Vec3> getSystemAccelerations(const std::vector<Body>& bodies)
{
    std::vector<Vec3> systemAccelerations(bodies.size());

    for ( std::size_t i = 0; i < bodies.size(); ++i )
    {
        for ( std::size_t j = 0; j < bodies.size(); ++j )
        {
            if ( j == i ) {
                continue;
            }

            systemAccelerations[i] += getGravitationlAcceleration(bodies[j], bodies[i]);
        }
    }

    return systemAccelerations;
}

/*
 * using LeapFrog Kick-Drift algorithm simulate updating velocity
 */
void simulateSystem(std::vector<Body>& bodies, double dt) 
{

    std::vector<Vec3> systemAccelerations = getSystemAccelerations(bodies);

    // half kick 1- update vel0city
    for ( std::size_t i = 0; i < bodies.size(); ++i ) 
    {
        bodies[i].velocity += systemAccelerations[i] * ( dt / 2.0);
    }

    // drift - update position
    for ( std::size_t i = 0; i < bodies.size(); ++i )
    {
        bodies[i].position += (bodies[i].velocity * dt);
    }

    // update accelerations
    systemAccelerations = getSystemAccelerations(bodies);

    // half kick 2 - update velocity
    for ( std::size_t i = 0; i < bodies.size(); ++i )
    {
        bodies[i].velocity += systemAccelerations[i] * ( dt / 2 );
    }

    // update time
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


    // make a world struct
    std::vector<Body> bodies = {sun, mercury};

    return 0;

} 
