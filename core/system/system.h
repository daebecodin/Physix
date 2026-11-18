/*
 * Initialize a system with varying precision though the class template Real
 * 
 */
#pragma once
#include <algorithm>
#include <span>
#include <stdexcept>
#include <vector>
#include "problem.h"
#include "integrators/euler.h"
#include "physics/nbody.h"
#include <cstddef>

template <class Real>
class System
{
    public:
        using real_type = Real; // precision type to be used in the simulation

        System(std::size_t N, Real G, Real C)
            :
                N_(N),
                G_(G),
                C_(C),
                t_(static_cast<Real>(0)),
                state_(6*N),
                derivatives_(6*N),
                mass_(N),
                radius_(N)
    {}

        explicit System(const Problem<Real>& problem)
            : System(problem.bodies.size(), problem.G, Real{})
        {
            t_ = problem.t0;
            for (std::size_t i = 0; i < N_; ++i)
            {
                const auto& body = problem.bodies[i];
                std::copy_n(body.position.data().begin(), 3, state_.begin() + 3*i);
                std::copy_n(body.velocity.data().begin(), 3, state_.begin() + 3*(N_ + i));
                mass_[i] = body.mass;
            }
        }

        void set_state(const std::vector<Real>& pos,
                const std::vector<Real>& vel,
                const std::vector<Real>& m,
                const std::vector<Real>& r)
        {
            if (pos.size() != 3*N_ || vel.size() != 3*N_ || m.size() != N_ || r.size() != N_)
                throw std::invalid_argument("Invalid system state sizes.");

            std::copy(pos.begin(), pos.end(), state_.begin());
            std::copy(vel.begin(), vel.end(), state_.begin() + 3*N_);
            mass_ = m;
            radius_ = r;
        }

        void step(Real dt)
        {
            sim::ode_n_body_first_order<Real>(state_, N_, G_, mass_, derivatives_);
            step_euler<Real>(state_, derivatives_, dt);
            t_ += dt;
        }

        // time, size, pos, vel

        Real time() const
        {
            return t_;
        }

        std::size_t size() const 
        {
            return N_;
        }

        std::span<const Real> positions() const
        {
            return std::span<const Real>(state_).first(3*N_);
        }

        std::span<const Real> velocities() const
        {
            return std::span<const Real>(state_).subspan(3*N_);
        }

    private:
        std::size_t N_;
        Real G_; // gravity constant
        Real C_; // light speed constant
        Real t_; // system time

        std::vector<Real> state_;
        std::vector<Real> derivatives_;
        std::vector<Real> mass_;
        std::vector<Real> radius_;

};
