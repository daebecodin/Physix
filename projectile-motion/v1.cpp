#include "pch.h"
#include <cmath>
#include <ios>
#include <numbers>
#include <array>
#include <limits>

using std::cout, std::cin;
using std::begin, std::end;

constexpr double GRAVITY = 9.8;

// math functions take radian 
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
    gravityFactor = -GRAVITY / 2;

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


std::vector<double> resolveComponents(double magnitude, double theta)
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
    vy = initialVy - (GRAVITY * timeOfFlight);

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
    deltaY = (initialVy * initialVy) / (2 * GRAVITY);
    return launchHeight + deltaY;
}

double getDuble(const std::string& prompt) 
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


int main() 
{

    double initialVelocity = getDuble("Initial Velocity");
    double theta = getDuble("Theta");
    double xInitial = getDuble("X-Initial");
    double yInitial = getDuble("Y-Initial");
    double yFinal = getDuble("Y-Final");

    std::vector<double> components = resolveComponents(initialVelocity, theta);
    double initialVx = components.at(0);
    double initialVy = components.at(1);

    double time = timeOfFlight(initialVy, yInitial, yFinal);

    double landingPoint = horizontalDisplacement(initialVx, time);

    std::vector<double>  landingComponents = landingVelocity(initialVx, initialVy, time);
    double landingVx = landingComponents.at(0);
    double landingVy = landingComponents.at(1);
    double impactSpeed = velocityMagnitude(landingVx, landingVy);

    double peakHeight = maximumHeight(initialVy, yInitial);



    cout << "Time: " << time << '\n';
    cout << "LandingPosition: " << landingPoint << '\n';
    cout << "Impact Speed: " << impactSpeed << '\n';
    cout << "Max Height: " << peakHeight << '\n';

    return 0;
}
