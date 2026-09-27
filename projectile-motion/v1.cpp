#include "pch.h"
#include <cmath>
#include <numbers>
using std::cout, std::cin;

double degreesToRadians(double theta)
{
    double pi = std::numbers::pi;

    double divisonFactor = (pi / 180);

    return theta * divisonFactor;
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

    std::vector<double> v1 = resolveComponents(10, 0);
    for (const double &component : v1)
    {
        cout << component << '\n';
    }
    return 0;

}
