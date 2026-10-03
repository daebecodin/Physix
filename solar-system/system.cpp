#include "pch.h"
#include "physix.h"
#include <stdexcept>
#include <chrono>
#include <thread>

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
    // Bodies
    /*
     * sun
     * mercury
     * venus
     * earth
     * mars
     * jupiter
     * saturn
     * uranus
     * neptune
     */
    Body sun {
        "Sun",
        SUN_MASS, 
        {0.0, 0.0, 0.0}, 
        {}
    };

    Body mercury {
        "Mercury",
        MERCURY_MASS, 
        {MERCURY_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body venus {
        "Venus",
        VENUS_MASS,
        {VENUS_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };
    
    Body earth {
        "Earth",
        EARTH_MASS,
        {EARTH_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body mars {
        "Mars",
        MARS_MASS,
        {MARS_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body jupiter {
        "Jupiter",
        JUPITER_MASS,
        {JUPITER_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body saturn {
        "Saturn",
        SATURN_MASS,
        {SATURN_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body uranus {
        "Uranus", 
        URANUS_MASS,
        {URANUS_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };

    Body neptune {
        "Neptune",
        NEPTUNE_MASS,
        {NEPTUNE_ORBITAL_DISTANCE, 0.0, 0.0},
        {}
    };


    // Gravitational Acceleration Pairs
    Vec3 mercuryAccelerationFromSun = getGravitationlAcceleration(sun, mercury);
    Vec3 sunAccelerationFromMercury = getGravitationlAcceleration(mercury, sun);
    
    // Circular Orbit Speeds and Initial Velocity
    double mercuryOrbitSpeed = getCircularOrbitSpeed(sun.mass, MERCURY_ORBITAL_DISTANCE);
    mercury.velocity = {0.0, mercuryOrbitSpeed, 0.0};

    double venusOrbitSpeed = getCircularOrbitSpeed(sun.mass, VENUS_ORBITAL_DISTANCE);
    venus.velocity = {0.0, venusOrbitSpeed, 0.0};

    double earthOrbitSpeed = getCircularOrbitSpeed(sun.mass, EARTH_ORBITAL_DISTANCE);
    earth.velocity = {0.0, earthOrbitSpeed, 0.0};

    double marsOrbitSpeed = getCircularOrbitSpeed(sun.mass, MARS_ORBITAL_DISTANCE);
    mars.velocity = {0.0, marsOrbitSpeed, 0.0};

    double jupiterOrbitSpeed = getCircularOrbitSpeed(sun.mass, JUPITER_ORBITAL_DISTANCE);
    jupiter.velocity = {0.0, jupiterOrbitSpeed, 0.0};

    double saturnOrbitSpeed = getCircularOrbitSpeed(sun.mass, SATURN_ORBITAL_DISTANCE);
    saturn.velocity = {0.0, saturnOrbitSpeed, 0.0};

    double uranusOrbitSpeed = getCircularOrbitSpeed(sun.mass, URANUS_ORBITAL_DISTANCE);
    uranus.velocity = {0.0, uranusOrbitSpeed, 0.0};

    double neptuneOrbitSpeed = getCircularOrbitSpeed(sun.mass, NEPTUNE_ORBITAL_DISTANCE);
    neptune.velocity = {0.0, neptuneOrbitSpeed, 0.0};



    // make a world struct
    std::vector<Body> bodies = {sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune};
    const std::vector<Body> startingBodies = bodies;

    double duration = 24.0 * 60.0 * 60.0; // 86,400 seconds
    double dt = 60.0;// 60 seconds per step

    std::size_t steps = static_cast<std::size_t>( duration / dt );


    std::cout << "Starting Positions\n";
    for ( const Body& body : startingBodies)
    {
        std::cout 
                << body.name << ": " 
                << body.position.x << ", "
                << body.position.y << ", "
                << body.position.z << ", "
                << '\n';

    }

    std::cout << "\nLive / Final Positions\n";
    for (std::size_t i = 0; i < steps; ++i) 
    {
        simulateSystem(bodies, dt);

        if (i > 0) 
        {
            std::cout << "\033[" << bodies.size() << "A";
        }
       

        for (const Body& body : bodies)
        {
            std::cout 
                << '\r' << "\033[2K" // Return to row start and erase it
                << body.name << ": " 
                << body.position.x << ", "
                << body.position.y << ", "
                << body.position.z << ", "
                << '\n';
        }

        // slows down the display for better visualization
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }




    return 0;

} 
