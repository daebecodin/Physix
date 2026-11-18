#include "system.h"
#include <matplot/matplot.h>

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

// Each orbit-demo figure has one axes. Gnuplot accepts Matplot++'s inline
// data in ordinary plot mode, so these figures need no multiplot wrapper.
class SingleAxesGnuplot : public matplot::backend::gnuplot
{
public:
    void run_command(const std::string& command) override
    {
        if (command == "set multiplot" || command == "unset multiplot")
            return;
        matplot::backend::gnuplot::run_command(command);
    }
};

using Triple = std::array<double, 3>;

struct Sample
{
    double time;
    std::array<Triple, 2> position;
    std::array<Triple, 2> velocity;
};

Sample sample(const System<double>& system)
{
    Sample result{system.time(), {}, {}};
    for (std::size_t i = 0; i < 2; ++i)
    {
        std::copy_n(system.positions().begin() + 3*i, 3, result.position[i].begin());
        std::copy_n(system.velocities().begin() + 3*i, 3, result.velocity[i].begin());
    }
    return result;
}

std::vector<Sample> integrate(const Problem<double>& problem, bool preallocate)
{
    System<double> system(problem);
    const auto steps = static_cast<std::size_t>(std::llround((problem.tf - problem.t0) / problem.dt));
    std::vector<Sample> history;
    if (preallocate)
        history.reserve(steps + 1);

    history.push_back(sample(system));
    for (std::size_t i = 0; i < steps; ++i)
    {
        system.step(problem.dt);
        history.push_back(sample(system));
    }
    return history;
}

Triple cross(const Triple& a, const Triple& b)
{
    return {a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]};
}

double squared_norm(const Triple& a)
{
    return a[0]*a[0] + a[1]*a[1] + a[2]*a[2];
}

struct Diagnostics
{
    double energy;
    double angular_momentum;
    double eccentricity;
};

Diagnostics diagnostics(const Sample& s, const Problem<double>& problem)
{
    const double m1 = problem.bodies[0].mass;
    const double m2 = problem.bodies[1].mass;
    Triple r{}, v{}, angular{};
    const Triple h1 = cross(s.position[0], s.velocity[0]);
    const Triple h2 = cross(s.position[1], s.velocity[1]);
    for (int i = 0; i < 3; ++i)
    {
        r[i] = s.position[0][i] - s.position[1][i];
        v[i] = s.velocity[0][i] - s.velocity[1][i];
        angular[i] = m1*h1[i] + m2*h2[i];
    }
    const double distance = std::sqrt(squared_norm(r));
    const double kinetic = (m1*squared_norm(s.velocity[0]) + m2*squared_norm(s.velocity[1])) / 2;
    Triple eccentricity = cross(v, cross(r, v));
    for (int i = 0; i < 3; ++i)
        eccentricity[i] = eccentricity[i] / (problem.G*(m1 + m2)) - r[i] / distance;
    return {kinetic - problem.G*m1*m2/distance, std::sqrt(squared_norm(angular)),
            std::sqrt(squared_norm(eccentricity))};
}

int main(int argc, char** argv)
{
    const std::array<BodyInitialState<double>, 2> bodies{{
        {{1, 0, 0}, {0, 0.5, 0}, 1},
        {{-1, 0, 0}, {0, -0.5, 0}, 1}
    }};
    auto problem = make_n_body_problem<double>(bodies, 1, 0, 15, 0.0001);
    std::vector<Sample> history;
    for (bool preallocate : {false, true})
    {
        constexpr int trials = 10;
        std::chrono::duration<double> total{};
        std::size_t samples = 0;
        for (int trial = 0; trial < trials; ++trial)
        {
            const auto start = std::chrono::steady_clock::now();
            auto run = integrate(problem, preallocate);
            total += std::chrono::steady_clock::now() - start;
            samples = run.size();
            if (preallocate && trial == trials - 1)
                history = std::move(run);
        }
        std::cout << (preallocate ? "Reserved" : "Growing") << " history: "
                  << samples << " samples, mean of " << trials << " runs: "
                  << total.count() / trials << " s\n";
    }

    auto orbit = matplot::figure<SingleAxesGnuplot>(true);
    for (int body = 0; body < 2; ++body)
    {
        std::vector<double> x, y;
        for (const auto& s : history)
        {
            x.push_back(s.position[body][0]);
            y.push_back(s.position[body][1]);
        }
        matplot::plot(x, y);
        matplot::hold(matplot::on);
    }
    matplot::axis(matplot::equal);
    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::title("Two-body orbit");

    const std::array figures{matplot::figure<SingleAxesGnuplot>(true), matplot::figure<SingleAxesGnuplot>(true), matplot::figure<SingleAxesGnuplot>(true)};
    problem.tf = 100;
    for (double dt : {0.001, 0.0001, 0.00001})
    {
        problem.dt = dt;
        System<double> system(problem);
        const auto initial = diagnostics(sample(system), problem);
        std::vector<double> time, error_time, energy, angular, eccentricity;
        // Sample every 0.1 time units for the comparison plots.
        const int stride = static_cast<int>(std::llround(0.1 / dt));
        for (int i = 0; i <= 1000; ++i)
        {
            const auto d = diagnostics(sample(system), problem);
            time.push_back(system.time());
            // The initial relative errors are zero and have no logarithm.
            if (i > 0)
            {
                error_time.push_back(system.time());
                energy.push_back(std::abs((d.energy - initial.energy) / initial.energy));
                angular.push_back(std::abs(d.angular_momentum / initial.angular_momentum - 1));
            }
            eccentricity.push_back(d.eccentricity);
            if (i < 1000)
                for (int step = 0; step < stride; ++step)
                    system.step(dt);
        }
        const std::array<const std::vector<double>*, 3> series{&energy, &angular, &eccentricity};
        const std::array titles{"Relative energy error", "Relative angular momentum error", "Eccentricity"};
        for (int i = 0; i < 3; ++i)
        {
            matplot::figure(figures[i]);
            matplot::hold(matplot::on);
            auto curve = i < 2 ? matplot::semilogy(error_time, *series[i])
                               : matplot::plot(time, *series[i]);
            curve->display_name("dt=" + std::to_string(dt));
            matplot::title(titles[i]);
            matplot::xlabel("Time");
            matplot::legend();
        }
    }
    if (argc == 2 && std::string(argv[1]) == "--energy")
    {
        figures[0]->show();
    }
    else if (argc == 2)
    {
        const std::string prefix = argv[1];
        orbit->save(prefix + "-orbit.png");
        const std::array suffix{"-energy.png", "-angular-momentum.png", "-eccentricity.png"};
        for (int i = 0; i < 3; ++i)
            figures[i]->save(prefix + suffix[i]);
    }
    else
    {
        orbit->show();
        for (const auto& figure : figures)
            figure->show();
    }
}
