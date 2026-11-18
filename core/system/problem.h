/*
 * Initial conditions and run settings for an N-body problem.
 */
#pragma once

#include <span>
#include <vector>

#include "bodies/vector3.h"

template <class Real>
struct BodyInitialState
{
    Vector<Real> position;
    Vector<Real> velocity;
    Real mass;
};

template <class Real>
struct Problem
{
    std::vector<BodyInitialState<Real>> bodies;
    Real G;
    Real t0;
    Real tf;
    Real dt;
};

/*
 * A span borrows the caller's bodies. They must stay alive during this call.
 * Problem owns a copy, so it can outlive the original array or vector.
 */
template <class Real>
Problem<Real> make_n_body_problem(
        std::span<const BodyInitialState<Real>> bodies,
        Real G, Real t0, Real tf, Real dt)
{
    return {
        std::vector<BodyInitialState<Real>>(bodies.begin(), bodies.end()),
        G,
        t0,
        tf,
        dt
    };
}
