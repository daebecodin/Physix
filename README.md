# Physix

Right now I have a v1 with the Sun and all eight planets. They pull on each other, and I use leapfrog to update their velocities and positions over time. The terminal shows the numbers updating live.

I'm learning as I build this. Getting numbers to change was the first step. Now I want to check that those numbers actually make sense.

## What it does rn

- Each body has a name, mass, position, and velocity.
- Each body gets pulled by every other body, excluding itself.
- I add those acceleration vectors to get the total acceleration for each body.
- Starting planet speeds come from the circular orbit equation.
- Leapfrog advances the system: half-kick, drift, recalculate acceleration, half-kick.
- The Sun moves too. It isn't held in place.
- I save the starting bodies so I can compare starting and ending values.
- The terminal prints starting positions, then updates simulated days, distance from the Sun, and y position in the same rows.

There is also a projectile-motion calculator and some force-related code in the repo.

## Running it

I use C++20. From `solar-system/`:

```sh
g++ -std=c++20 -I../include system.cpp ../impl/physix.cpp ../impl/physix_calculations.cpp -o out
./out
```

Or from the project root with CMake:

```sh
cmake -S . -B build
cmake --build build --target system
./build/solar-system/system
```

Shared calculations and the leapfrog step live in `impl/`, with declarations in `include/`. CMake builds them as the `physix` library and links the example programs to it.

To build and run the projectile calculator from the root:

```sh
cmake --build build --target v1
./build/projectile-motion/v1
```

Use a terminal that supports ANSI cursor movement. The rows need to fit on screen without wrapping, or the live output gets messed up.

## How it's set up

The physics uses kilograms, meters, and seconds. I convert distances to kilometers for the live display.

All planets currently start on the positive x-axis, with velocity in positive y. Their z positions and velocities are zero, so everything stays in the same plane. The Sun starts at the origin with zero velocity, then moves from the planets pulling on it.

The current run is one simulated day, with `dt = 60` seconds. That's 1,440 steps. I pause for 50 milliseconds after each display update so I can actually watch the numbers change. The pause affects playback, not the physics.

`Physix::Orbital::distanceFromSun` uses the Sun at index 0 and measures distance from its center.

## Math so far

Acceleration magnitude from one attracting body:

$$
a=\frac{GM}{r^2}
$$

Here, `M` is the attractor's mass and `r` is the distance between the bodies' centers.

To give that acceleration direction, I use the displacement components pointing toward the attractor:

$$
a_x=a\frac{d_x}{r},\qquad a_y=a\frac{d_y}{r},\qquad a_z=a\frac{d_z}{r}
$$

I do that for every other body and add the results.

Circular starting speed:

$$
v=\sqrt{\frac{GM}{r}}
$$

The starting velocity is perpendicular to the direction toward the Sun. Leapfrog then updates all bodies in separate stages so the gravity calculations use positions from the same moment.

## What isn't finished

This is a simplified starting system. I haven't recreated the actual planets' positions and velocities for a specific date yet.

- Every planet starts lined up, with an approximate orbital size and a circular starting speed based only on the Sun. Once they all interact, the orbits won't necessarily stay perfectly circular.
- The masses and orbital sizes are approximate constants in the code.
- Total starting momentum isn't zero. That means the center of mass moves, which isn't automatically a physics bug.
- Bodies are point masses. I reject zero separation, but I haven't handled close encounters or collisions.
- The time step is fixed. I still need to check how much error it introduces.
- The one-day run has passed basic runtime checks. I haven't verified accuracy over many orbits yet.
- No moons, belts, asteroids, relativity, or graphics yet.

## What I want to add later

- **NASA JPL Horizons:** get actual starting position and velocity vectors for the same date and reference frame. Convert the units correctly, record the time scale, and get mass data separately where needed.
- Compare my simulated positions with Horizons data, while accounting for the simpler physics in my model.
- Load a system from a file instead of writing every body into the code.
- Export data to CSV so I can plot the orbits and check results.
- Reset, duration settings, and playback speed controls. Playback speed should be separate from the physics time step.
- Moons, asteroids, belts, and custom systems, once I have a plan for their interactions.
- OpenGL to draw the positions the simulator already calculates.

## Resources

- Giancoli, *Physics: Principles with Applications*, 7th edition: Chapters 5–8.
- [NASA planetary data](https://nssdc.gsfc.nasa.gov/planetary/factsheet/)
- [NASA Sun data](https://nssdc.gsfc.nasa.gov/planetary/factsheet/sunfact.html)
- [NASA JPL Horizons](https://ssd.jpl.nasa.gov/horizons/)
- [Horizons manual](https://ssd.jpl.nasa.gov/horizons/manual.html)
- [Leapfrog solar system walkthrough](https://amor.cms.hu-berlin.de/~rodrigus/Jupyter%20Notebooks/Solar_System_N-Body_Simulation.html)
