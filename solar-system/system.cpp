#include "pch.h"
#include "physix.h"
#include "physix_calulations.h"
#include "physix_constants.h"
#include "physix_input.h"
#include <stdexcept>
#include <chrono>
#include <thread>
#include <iomanip>




int main() 
{
    namespace Masses = Physix::Masses;
    namespace Distances = Physix::Distances;
    namespace Calculate = Physix::Orbital;
    namespace System = Physix;


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
    double mercuryOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::MERCURY_ORBITAL_DISTANCE);
    mercury.velocity = {0.0, mercuryOrbitSpeed, 0.0};

    double venusOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::VENUS_ORBITAL_DISTANCE);
    venus.velocity = {0.0, venusOrbitSpeed, 0.0};

    double earthOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::EARTH_ORBITAL_DISTANCE);
    earth.velocity = {0.0, earthOrbitSpeed, 0.0};

    double marsOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::MARS_ORBITAL_DISTANCE);
    mars.velocity = {0.0, marsOrbitSpeed, 0.0};

    double jupiterOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::JUPITER_ORBITAL_DISTANCE);
    jupiter.velocity = {0.0, jupiterOrbitSpeed, 0.0};

    double saturnOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::SATURN_ORBITAL_DISTANCE);
    saturn.velocity = {0.0, saturnOrbitSpeed, 0.0};

    double uranusOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::URANUS_ORBITAL_DISTANCE);
    uranus.velocity = {0.0, uranusOrbitSpeed, 0.0};

    double neptuneOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::NEPTUNE_ORBITAL_DISTANCE);
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
        System::simulateSystem(bodies, dt);
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
                << Calculate::distanceFromSun(bodies, j) / 1000.0
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
