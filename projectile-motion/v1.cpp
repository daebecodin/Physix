#include "pch.h"
#include <cmath>
#include <numbers>
#include <array>

using std::cout, std::cin;

// math functions take radians 
double degreesToRadians(double theta)
{
    double pi = std::numbers::pi;

    double divisonFactor = (pi / 180);

    return theta * divisonFactor;
}

std::vector<double> solveQuadritic(double a, double b, double c)
{
    double discriminent;
    double x1, x2;

    std::vector<double> roots;

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
        x2 = x1;
        roots.push_back(x1);
    }

    return roots;
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


int main() 
{

    std::vector<double> v1 = resolveComponents(10, 45);
    for (const double &component : v1)
    {
        cout << component << '\n';
    }

    std::vector<double> v2 = solveQuadritic(1, -5, 6);
    for (const double &root : v2)
    {
        cout << root << '\n';
    }
    return 0;

}
