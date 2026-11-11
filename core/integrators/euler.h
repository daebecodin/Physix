/*
 * Integrator Functions
 *
 * We pass all position and velocity components as state through an std::span
 * 
 */

#include <span>


template <class Real>
void step_euler(
        std::span<Real> sim_state,
        std::span<const Real> sim_masses,
        std::span<Real> derivatives,
        Real grav_constant,
        Real dt

);

