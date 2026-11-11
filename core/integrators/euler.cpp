/*
 * 
 */
#include <cstddef>
#include <span>
#include <stdexcept>
#include "euler.h"
#include "nbody.h"

/* @brief One step in the Euler integration
 * 
 * @param sim_state Initial Conditions
 * @param sim_masses Total masses in a system
 * @param derivatives Calculated derivatives for each mass
 *          - returns 3 velocity components
 *          - returns 3 acceleration components
 * @param Universal Gravitational Constant
 * @param Change in time
 */

template <class Real>
void step_euler(
        std::span<Real> sim_state,
        std::span<const Real> sim_masses,
        std::span<Real> derivatives,
        Real grav_constant,
        Real dt
        )
{

    const std::size_t total_bodies = sim_masses.size();
    /*
     * checks for valid velocity and acceleration components
     *
     * TODO: Make custom exception
     */
    if (sim_state.size() != 6 * total_bodies || derivatives.size() != sim_state.size())
    {
        throw std::invalid_argument("Invalid Euler buffer sizes.");
    }

    /*
     * Calculate the derivatives with the current system state 
     *
     */
    


}

