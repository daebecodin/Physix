/*
 * Initialize a system with varying precision though the class template Real
 * 
 */
#pragma once
#include <vector>
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
                pos_(3*N),
                vel_(3*N),
                mass_(N),
                radius_(N)
    {}

        /*
         * Takes data from the caller to initialize the systems internal arrays
         *
         * TODO: rewrite to assert or thow
         */
        void set_state(const std::vector<Real>& pos,
                const std::vector<Real>& vel,
                const std::vector<Real>& m,
                const std::vector<Real>& r)
        {
            // simple, defensive version
            if (pos.size() == pos_.size()) pos_ = pos;
            if (vel.size() == vel_.size()) vel_ = vel;
            if (m.size()  == mass_.size())   mass_ = m;
            if (r.size() == radius_.size())   radius_ = r;
        }

        void step(Real dt)
        {
            // TODO: Call integrator
            // integrator_step()

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

        const std::vector<Real>& positions() const 
        {
            return pos_;
        }

        const std::vector<Real>& velocities() const
        {
            return vel_;
        }

    private:
        std::size_t N_;
        Real G_; // gravity constant
        Real C_; // light speed constant
        Real t_; // system time

        std::vector<Real> pos_;
        std::vector<Real> vel_;
        std::vector<Real> mass_;
        std::vector<Real> radius_;

};
