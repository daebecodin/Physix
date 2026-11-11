#include "orbital_calculations.h"
#include "constants.h"
#include <cmath>
#include <stdexcept>

namespace Gravity = Physix::GravitationalForces;

namespace Physix {
    namespace Orbital {

        double delta(double d2, double d1)
        {
            return d2 - d1;
        }

        double distanceMagnitude(double dx, double dy, double dz)
        {
            return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
        }

        double accelerationMagnitude(double attracterMass, double distance)
        {
            return (Gravity::UNIVERSAL_GRAVITY * attracterMass) / (distance * distance);
        }

        double circularOrbitSpeed(double attrcterMass, double distance)
        {
            return std::sqrt((Gravity::UNIVERSAL_GRAVITY * attrcterMass) / distance);
        }

        Vec3 vectorComponents(double accelerationMagnitude, double dx, double dy, double dz)
        {
            double r = distanceMagnitude(dx, dy, dz);

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
        Vec3 gravitationlAcceleration(const Body& attracter, const Body& attracted){

            double dx = delta(attracter.position.getX(), attracted.position.getX());
            double dy = delta(attracter.position.getY(), attracted.position.getY());
            double dz = delta(attracter.position.getZ(), attracted.position.getZ());


            // magnitude of the difference of between centers
            double r = distanceMagnitude(dx, dy, dz);

            if (r == 0) 
            {
                throw std::invalid_argument("Bodies must have non-zero separations");
            }


            double a = accelerationMagnitude(attracter.mass, r);

            return vectorComponents(a, dx, dy, dz);
        }

        double distanceFromSun(const std::vector<Body>& bodies, std::size_t bodyIndex)
        {
            double dx = bodies[bodyIndex].position.getX() - bodies[0].position.getX();
            double dy = bodies[bodyIndex].position.getY() - bodies[0].position.getY();
            double dz = bodies[bodyIndex].position.getZ() - bodies[0].position.getZ();

            return distanceMagnitude(dx, dy, dz);
        }

        /*
         * sums the gravitational acceleration vectors influenced by each surrounding body for each body,
         * excluding itself
         */
        std::vector<Vec3> systemAccelerations(const std::vector<Body>& bodies)
        {
            std::vector<Vec3> systemAccelerations(bodies.size());

            for ( std::size_t i = 0; i < bodies.size(); ++i )
            {
                for ( std::size_t j = 0; j < bodies.size(); ++j )
                {
                    if ( j == i ) {
                        continue;
                    }

                    systemAccelerations[i] += gravitationlAcceleration(bodies[j], bodies[i]);
                }
            }

            return systemAccelerations;
        }

    }
}
