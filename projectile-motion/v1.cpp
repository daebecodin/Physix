#include "pch.h"
#include <cmath>
#include <numbers>
#include <array>

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


int main() 
{

    cout << "components\n";
    std::vector<double> v1 = resolveComponents(10, 45);
    for (const double &component : v1)
    {
        cout << component << '\n';
    }

    cout << "quadritic\n";
    std::vector<double> v2 = solveQuadratic(0, 5, 2);
    for (const double &root : v2)
    {
        cout << root << '\n';
    }
    cout << v2.size() << '\n';

    cout << "time of flight\n";
    double t = timeOfFlight(10, 15, 0);
    cout << t << '\n';


    cout << "landing position\n";
    double landingPos = horizontalDisplacement(8, 3);
    cout << landingPos << '\n';

    cout << "landing velocity\n";
    std::vector<double> landingV = landingVelocity(8, 10, 2);
    double vx = landingV.at(0);
    double vy = landingV.at(1);
    cout << "Vx -> " << vx << '\n';
    cout << "Vy -> " << vy << '\n';

    cout << "speed\n";
    double speed = velocityMagnitude(3, 4);
    cout << speed << '\n';


    cout << "max height\n";
    double maxHeight = maximumHeight(9.8, 10);
    cout << maxHeight << '\n';
    return 0;
}
