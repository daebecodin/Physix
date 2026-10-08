#include "physix.h"
#include <vector>
#include "physix_calulations.h"

namespace Calculate = Physix::Orbital;

namespace Physix {
/*
 * using Leap Frog Kick-Drift algorithm simulate updating velocity
 */
void simulateSystem(std::vector<Body>& bodies, double dt) 
{

    std::vector<Vec3> totalAcceleration = Calculate::systemAccelerations(bodies);

    // half kick 1- update velocity
    for ( std::size_t i = 0; i < bodies.size(); ++i ) 
    {
        bodies[i].velocity += totalAcceleration[i] * ( dt / 2.0);
    }

    // drift - update position
    for ( std::size_t i = 0; i < bodies.size(); ++i )
    {
        bodies[i].position += (bodies[i].velocity * dt);
    }

    // update accelerations
    totalAcceleration = Calculate::systemAccelerations(bodies);

    // half kick 2 - update velocity
    for ( std::size_t i = 0; i < bodies.size(); ++i )
    {
        bodies[i].velocity += totalAcceleration[i] * ( dt / 2 );
    }

    // update time
}

}
