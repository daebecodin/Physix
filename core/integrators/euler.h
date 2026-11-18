#pragma once

#include <span>
#include <stdexcept>

// Advance with derivatives evaluated at the beginning of the step.
template <class Real>
void step_euler(std::span<Real> state, std::span<const Real> derivatives, Real dt)
{
    if (state.size() != derivatives.size())
        throw std::invalid_argument("Invalid Euler buffer sizes.");

    for (std::size_t i = 0; i < state.size(); ++i)
        state[i] += dt * derivatives[i];
}
