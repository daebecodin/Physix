#include <iostream>
#include <stdexcept>
#include "physix_calulations.h"
#include "physix_input.h"

using std::cout;

int main() try
{

    namespace Calculate = Physix::Projectile;
    namespace Input = Physix::Input;

    double initialVelocity = Input::getDouble("Initial Velocity");
    double theta = Input::getDouble("Theta");
    double xInitial = Input::getDouble("X-Initial");
    double yInitial = Input::getDouble("Y-Initial");
    double yFinal = Input::getDouble("Y-Final");

    std::vector<double> components = Calculate::vectorComponents(initialVelocity, theta);
    double initialVx = components.at(0);
    double initialVy = components.at(1);

    double time = Calculate::timeOfFlight(initialVy, yInitial, yFinal);

    if (time < 0)
    {
        throw std::runtime_error("The projectile does not reach the requested final height");
    }

    double landingPoint = xInitial + Calculate::horizontalDisplacement(initialVx, time);

    std::vector<double>  landingComponents = Calculate::landingVelocity(initialVx, initialVy, time);
    double landingVx = landingComponents.at(0);
    double landingVy = landingComponents.at(1);
    double impactSpeed = Calculate::velocityMagnitude(landingVx, landingVy);

    double peakHeight = Calculate::maximumHeight(initialVy, yInitial);



    cout << "Time: " << time << '\n';
    cout << "LandingPosition: " << landingPoint << '\n';
    cout << "Impact Speed: " << impactSpeed << '\n';
    cout << "Max Height: " << peakHeight << '\n';

    return 0;
}

catch (const std::exception& error)
{
    std::cerr << error.what() << '\n';
    return 1;
}
