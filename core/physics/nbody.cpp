#include <span>
#include <cmath>
#include <stdexcept>
#include "nbody.h"

namespace sim
{
    template <class Real>
        void ode_n_body_first_order(
                std::span<const Real> state,
                std::size_t N,
                Real G,
                std::span<const Real> masses,
                std::span<Real> dxdt
                )
        {
            if (masses.size() != N || state.size() / 6 != N || state.size() % 6 != 0 ||
                dxdt.size() != state.size())
                throw std::invalid_argument("Invalid N-body buffer sizes.");

            Real x, y, z;
            Real dx, dy, dz;
            Real ax, ay, az;
            Real dist2; // distance squared between 2 planets: r^2
            Real dist3; // distance cubed between 2 planets: r^3
            Real grav_factor; // gravity constant * mass of another body, used for gravity between 2 bodies


            /*
             * Here we assign the velocity vectors to the position derivative of an indexed body  
             */
            for (std::size_t i = 0; i < 3 * N; i++)
            {
                dxdt[i] = state[3 * N + i];
            }

            /*
             *  Loop though all bodies, treating each as the target body j
             *  Calculate gravity between the target and every other mass in the system
             *  Calculate the acceleration vector of the target and store the components as time derivatives of velocity
             */
            for (std::size_t j = 0; j < N; j++)
            {
                // read position components
                x = state[j * 3];
                y = state[j * 3 + 1];
                z = state[j * 3 + 2];

                // initially set acceleration to 0
                ax = 0.0;
                ay = 0.0;
                az = 0.0;


                /*
                 * inner loop for gravitational interaction between 2 bodies
                 *
                 * skipping any massless or indentical bodies
                 */
                for (std::size_t k = 0; k < N; k++)
                {
                    if ((j == k) || (masses[k] == 0)) 
                    {
                        continue;
                    }

                    // the attraction force form onto the target body
                    grav_factor = G * masses[k];

                    // displacement vectors between the target and the attractor 
                    dx = x - state[k * 3];
                    dy = y - state[k * 3 + 1];
                    dz = z - state[k * 3 + 2];

                    // relative distances between 2 planets
                    dist2 = (dx * dx) + (dy * dy) + (dz * dz);
                    dist3 = std::sqrt(dist2) * dist2;

                    // use distance to find gravitational acceleration components of the target body
                    ax -= (grav_factor * dx / dist3);
                    ay -= (grav_factor * dy / dist3);
                    az -= (grav_factor * dz / dist3);
                }

                /* store the gravitational acceleration components into the part of dxdt that represents
                 * the derivative of velocity for the target body
                */ 
                dxdt[(N + j) * 3] = ax;
                dxdt[(N + j) * 3 + 1] = ay;
                dxdt[(N + j) * 3 + 2] = az;
            }
        }

};

template void sim::ode_n_body_first_order<float>(
        std::span<const float>, std::size_t, float, std::span<const float>, std::span<float>);

template void sim::ode_n_body_first_order<double>(
        std::span<const double>, std::size_t, double, std::span<const double>, std::span<double>);
