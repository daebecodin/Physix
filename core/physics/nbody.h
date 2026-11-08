/*
 * 
 */
#include <span>

namespace sim 
{

    template <class Real>
        std::size_t ode_n_body_first_order(
                std::span<Real> pos,
                Real G,
                std::span<const Real> masses,
                std::span<Real> dxdt
                );

    template <class Real>
        std::size_t calculate_accelerations(
                std::span<const Real> pos,
                std::span<const Real> vel,
                Real G,
                std::span<const Real> masses,
                std::span<const Real> radii,
                std::span<Real> acc
        );
};
