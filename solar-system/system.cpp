#include "pch.h"
#include "physix.h"
#include "physix_constants.h"
#include <stdexcept>
#include <chrono>
#include <thread>
#include <iomanip>

namespace Gravity = Physix::Constants::GravitationalForces;

double getDouble(const std::string& prompt) 
{
    double value;
    
    while (true)
    {
    std::cout << prompt << ": ";
    
    if (std::cin >> value)
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
    else
    {
        std::cout << "Invalud input, please again\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());
        std::cout << prompt << ": ";
    }

    }
}

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
    return (Gravity::UNIVERSAL_GRAVITY * attracterMass) / (distance * distance);
}

double getCircularOrbitSpeed(double attrcterMass, double distance)
{
    return std::sqrt((Gravity::UNIVERSAL_GRAVITY * attrcterMass) / distance);
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

double getDistanceFromSun(const std::vector<Body>& bodies, std::size_t bodyIndex)
{
    double dx = bodies[bodyIndex].position.x - bodies[0].position.x;
    double dy = bodies[bodyIndex].position.y - bodies[0].position.y;
    double dz = bodies[bodyIndex].position.z - bodies[0].position.z;

    return getDistanceMagnitude(dx, dy, dz);
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
 * using Leap Frog Kick-Drift algorithm simulate updating velocity
 */
void simulateSystem(std::vector<Body>& bodies, double dt) 
{

    std::vector<Vec3> systemAccelerations = getSystemAccelerations(bodies);

    // half kick 1- update velocity
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
    namespace Masses = Physix::Constants::Masses;
    namespace Distances = Physix::Constants::Distances;


      Body sun {
        "Sun",
        Masses::SUN_MASS, 
        {0.0, 0.0, 0.0}, 
        {}
    };

    Body mercury {
        "Mercury",
        Masses::MERCURY_MASS, 
        {Distances::MERCURY_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body venus {
        "Venus",
        Masses::VENUS_MASS,
        {Distances::VENUS_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };
    
    Body earth {
        "Earth",
        Masses::EARTH_MASS,
        {Distances::EARTH_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body mars {
        "Mars",
        Masses::MARS_MASS,
        {Distances::MARS_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body jupiter {
        "Jupiter",
        Masses::JUPITER_MASS,
        {Distances::JUPITER_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body saturn {
        "Saturn",
        Masses::SATURN_MASS,
        {Distances::SATURN_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body uranus {
        "Uranus", 
        Masses::URANUS_MASS,
        {Distances::URANUS_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body neptune {
        "Neptune",
        Masses::NEPTUNE_MASS,
        {Distances::NEPTUNE_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };
    
    // Circular Orbit Speeds and Initial Velocity
    double mercuryOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::MERCURY_ORBITAL_DISTANCE);
    mercury.velocity = {0.0, mercuryOrbitSpeed, 0.0};

    double venusOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::VENUS_ORBITAL_DISTANCE);
    venus.velocity = {0.0, venusOrbitSpeed, 0.0};

    double earthOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::EARTH_ORBITAL_DISTANCE);
    earth.velocity = {0.0, earthOrbitSpeed, 0.0};

    double marsOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::MARS_ORBITAL_DISTANCE);
    mars.velocity = {0.0, marsOrbitSpeed, 0.0};

    double jupiterOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::JUPITER_ORBITAL_DISTANCE);
    jupiter.velocity = {0.0, jupiterOrbitSpeed, 0.0};

    double saturnOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::SATURN_ORBITAL_DISTANCE);
    saturn.velocity = {0.0, saturnOrbitSpeed, 0.0};

    double uranusOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::URANUS_ORBITAL_DISTANCE);
    uranus.velocity = {0.0, uranusOrbitSpeed, 0.0};

    double neptuneOrbitSpeed = getCircularOrbitSpeed(sun.mass, Distances::NEPTUNE_ORBITAL_DISTANCE);
    neptune.velocity = {0.0, neptuneOrbitSpeed, 0.0};



    // make a world struct
    std::vector<Body> bodies = {sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune};
    const std::vector<Body> startingBodies = bodies;

    double duration = 24.0 * 60.0 * 60.0; // 86,400 seconds
    double dt = 60.0;// 60 seconds per step
    double elapsedTime = 0.0;

    std::size_t steps = static_cast<std::size_t>( duration / dt );

    std::cout << "\nStarting Positions (m)\n"
        << std::left << std::setw(8) << "Body"
        << std:: right << std::setw(14) << "X Pos" 
        << std::setw(14) << "Y pos" 
        << std::setw(14) << "Z pos" << '\n';

    for (const Body& body : startingBodies)
    {
        std::cout << std::left << std::setw(8) << body.name
            << std::right << std::scientific << std::setprecision(4)
            << std::setw(14) << body.position.x
            << std::setw(14) << body.position.y
            << std::setw(14) << body.position.z << '\n';
    }

    std::cout << '\n'
          << std::left << std::setw(8) << "Body"
          << std::right << std::setw(14) << "Sun dist (km)"
          << std::setw(14) << " X pos (km)"
          << std::setw(14) << "Y pos (km)" << '\n';

   for (std::size_t i = 0; i < steps; ++i)
   {
       simulateSystem(bodies, dt);
       elapsedTime += dt;

       if (i > 0) 
       {
           std::cout << "\033[" << bodies.size() + 1 << "A";
       }

       std::cout << '\r' << "\033[2K"
           << "Days: " 
           << std::fixed << std::setprecision(3)
           << elapsedTime / 86400.0 << '\n';

       for (std::size_t j = 0; j < bodies.size(); ++j)
       {
           std::cout << '\r' << "\033[2K"
               << std::left << std::setw(8) << bodies[j].name
               << std::right << std::scientific
               << std::setprecision(4)
               << std::setw(14)
               << getDistanceFromSun(bodies, j) / 1000.0
               << std::setw(14)
               << bodies[j].position.x / 1000.0
               << std::setw(14)
               << bodies[j].position.y / 1000.0
               << '\n';
    }

    std::cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
    return 0;

} 
