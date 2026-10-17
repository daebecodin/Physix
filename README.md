# Physix

Right now I have a v1 with the Sun and all eight planets. They pull on each other, and I use leapfrog to update their velocities and positions over time. An SDL3 window draws the bodies, and the terminal shows the numbers updating live.

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

## Build and run

Run these commands from the project root (`physix/`). If you're currently in `solar-system/` or `projectile-motion/`, run `cd ..` first.

### First build with CMake

Requires CMake 3.27 or newer and a C++20 compiler. The current `CMakeLists.txt` builds SDL3 from `/Users/dmac/SDL`; that source directory must exist. Update that path in `CMakeLists.txt` if SDL lives elsewhere.

```sh
cmake -S . -B build
cmake --build build --target system v1 -j 4
```

The first build also compiles SDL3, so it takes longer. Shared calculations and leapfrog live in `impl/`, with headers in `include/`. Both programs link the `physix` library.

### Solar-system simulation

Build the latest code, then launch the SDL window and live terminal output:

```sh
cmake --build build --target system -j 4
./build/solar-system/system
```

Close the window to quit. The physics stops advancing at 365 simulated days; the window stays open. Use a terminal that supports ANSI cursor movement, with enough room for the output rows.

### Projectile-motion calculator

```sh
cmake --build build --target v1 -j 4
./build/projectile-motion/v1
```

Enter initial speed (m/s), launch angle (degrees), initial x (m), initial y (m), and final y (m), in that order. For example, `10`, `45`, `0`, `0`, `0` gives about 1.443 seconds of flight, a landing x of 10.204 m, and an impact speed of 10 m/s.

### Refresh after editing

Close a running app, rebuild its target using the commands above, then launch it again. Rebuilding does not update an already running process.

To rebuild both programs:

```sh
cmake --build build --target system v1 -j 4
```

After changing CMake settings or adding source files, reconfigure and rebuild:

```sh
cmake -S . -B build
cmake --build build --target system v1 -j 4
```

For a clean rebuild of all targets in this build directory:

```sh
cmake --build build --clean-first -j 4
```

### Direct g++ builds on this Mac

These alternatives also run from the project root. The solar-system command uses the installed SDL3 headers and library under `/opt/homebrew`, separately from the SDL source used by CMake.

Solar system:

```sh
g++ -std=c++20 solar-system/system.cpp impl/physix.cpp impl/physix_calculations.cpp -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL3 -o solar-system/out
./solar-system/out
```

Projectile calculator:

```sh
g++ -std=c++20 projectile-motion/v1.cpp impl/physix_calculations.cpp impl/physix_input.cpp -Iinclude -o projectile-motion/out
./projectile-motion/out
```

Rerun the corresponding compiler command after edits, then launch the executable again.

## How it's set up

The physics uses kilograms, meters, and seconds. I convert distances to kilometers for the live display.

All planets currently start on the positive x-axis, with velocity in positive y. Their z positions and velocities are zero, so everything stays in the same plane. The Sun starts at the origin with zero velocity, then moves from the planets pulling on it.

The current run is one simulated year (365 days), with `dt = 60` seconds. That's 525,600 physics steps, with up to 10,000 steps per frame. There is a 50-millisecond pause after each display update. Steps per frame and the pause affect playback speed; `dt` controls the physics time step.

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
- No moons, belts, asteroids, or relativity yet. Graphics are a basic SDL3 body display.

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
