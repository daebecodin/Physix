#include "physix_calulations.h"
#include "physix_constants.h"
#include <cmath>
#include <iostream>
#include <numbers>
#include <stdexcept>

namespace Gravity = Physix::GravitationalForces;

namespace Physix {

    namespace Projectile {

        double degreesToRadians(double theta)
        {
            double pi = std::numbers::pi;

            double divisonFactor = (pi / 180);

            return theta * divisonFactor;
        }

        std::vector<double> solveQuadratic(double a, double b, double c)
        {

            double discriminent;
            double x1, x2;

            std::vector<double> roots;

            if (a == 0) 
            {
                std::cout << "a cannot be 0\n";
                return roots;
            }

            discriminent = b*b - 4*a*c;

            if (discriminent > 0) // real and different roots
            {
                x1 = (-b + std::sqrt(discriminent)) / (2*a);
                x2 = (-b - std::sqrt(discriminent)) / (2*a);

                roots.push_back(x1);
                roots.push_back(x2);
            }
            else if (discriminent == 0) // real and equal roots
            {
                x1 = -b / (2*a);
                roots.push_back(x1);
            }

            return roots;
        }

        double timeOfFlight(double verticalComponent, double yInitial, double yFinal)
        {
            double t {-1.0};
            double gravityFactor;
            double deltaY = -(yFinal - yInitial);
            gravityFactor = - Gravity::EARTH_GRAVITY/ 2;

            std::vector<double> times = solveQuadratic(gravityFactor, verticalComponent, deltaY);

            for (auto it = times.begin(); it != times.end(); ++it) 
            {
                double currentRoot = *it;
                if (currentRoot > t && currentRoot >= 0)
                {
                    t = *it;
                }
            }


            return t;
        }


        std::vector<double> vectorComponents(double magnitude, double theta)
        {
            double convertedTheta = degreesToRadians(theta);

            double vx = magnitude * std::cos(convertedTheta);
            double vy = magnitude * std::sin(convertedTheta);

            std::vector<double> vecComponents;
            vecComponents.push_back(vx);
            vecComponents.push_back(vy);

            return vecComponents;
        }

        double horizontalDisplacement(double horizontalComponent, double timeOfFlight)
        {
            return horizontalComponent * timeOfFlight;
        }

        std::vector<double> landingVelocity(double initialVx, double initialVy, double timeOfFlight)
        {
            double vx;
            double vy;

            vx = initialVx;
            vy = initialVy - (Gravity::EARTH_GRAVITY * timeOfFlight);

            std::vector<double> velocityComponents;
            velocityComponents.push_back(vx);
            velocityComponents.push_back(vy);

            return velocityComponents;
        }

        double velocityMagnitude(double vx, double vy)
        {
            return std::sqrt((vx*vx) + (vy*vy));
        }

        double maximumHeight(double initialVy, double launchHeight)
        {
            double deltaY;
            deltaY = (initialVy * initialVy) / (2 * Gravity::EARTH_GRAVITY);
            return launchHeight + deltaY;
        }


    }

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

            double dx = delta(attracter.position.x, attracted.position.x);
            double dy = delta(attracter.position.y, attracted.position.y);
            double dz = delta(attracter.position.z, attracted.position.z);


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
            double dx = bodies[bodyIndex].position.x - bodies[0].position.x;
            double dy = bodies[bodyIndex].position.y - bodies[0].position.y;
            double dz = bodies[bodyIndex].position.z - bodies[0].position.z;

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
