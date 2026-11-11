#include "projectile_calculations.h"
#include "constants.h"
#include <cmath>
#include <iostream>
#include <numbers>

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

}
