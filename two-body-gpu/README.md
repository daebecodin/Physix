# SDL3 GPU two-body example

This example starts with two lit 3D spheres in SDL3 GPU. A compute shader advances every body with mutual gravity and the kick–drift–kick leapfrog sequence used by `Physix::simulateSystem`. The rendering shader reads the updated GPU state directly; there is no per-frame CPU readback.

## Build and run

From the Physix repository root:

```sh
cmake -S . -B build
cmake --build build --target two_body_gpu -j 4
./build/two-body-gpu/two_body_gpu
```

This uses the repository's existing SDL source checkout at `/Users/dmac/SDL`. macOS/Metal can run with the included MSL source shaders. The executable loads shaders from `build/two-body-gpu/shaders/`, which CMake refreshes on each build. On Vulkan and D3D12, install the SDL_shadercross command-line tool, run `./two-body-gpu/compile-shaders.sh SPIRV` or `./two-body-gpu/compile-shaders.sh DXIL`, then rebuild. The script also accepts `MSL` and `all`. DXIL compilation requires the DirectX Shader Compiler dependencies available to shadercross. The HLSL files are the portable shader source; the checked-in MSL files let this Mac build run without a shader compiler.

**Controls:** Space pauses or resumes, R resets the bodies, Esc or window close quits.

## Physics and view

The Sun uses `Physix::Masses::SUN_MASS`; the Earth uses `Physix::Masses::EARTH_MASS` and `Physix::Distances::EARTH_ORBITAL_DISTANCE`. The starting relative speed comes from `Physix::Orbital::circularOrbitSpeed` with the combined mass. The Sun receives the opposite velocity scaled by the mass ratio, so the starting momentum is zero. The compute shader converts Physix's gravitational constant to solar masses, AU, and days and performs two 0.5-day steps per displayed frame. For each step, one GPU pass calculates half-step velocities and new positions from the old state; a second pass calculates acceleration at all new positions and finishes the velocities.

## Add a body

Add one `BodyState` entry to `initial_states` near the top of `two_body_gpu.cpp`. Its three groups are `{x, y, z, mass}`, `{vx, vy, vz, 0}`, and `{radius, red, green, blue}`. Position and radius are in AU, mass is in solar masses, and velocity is in AU per day. The radius is a visual size; it does not affect gravity. The array size sets the buffer capacity, compute body count, and number of rendered spheres automatically. Rebuild and run using the commands above. `R` restores all bodies to their initial states.

The simulation uses 32-bit floats on the GPU. This is a visual demo, not a high-precision orbital integrator. The spheres' radii are greatly exaggerated and do not represent physical diameters. The camera is fixed above the orbital plane with perspective and depth testing. Resize the window to change its aspect ratio. A frame advances one simulated day, so playback speed depends on display frame rate; the numerical step remains 0.5 day.

## Files

- `two_body_gpu.cpp`: SDL setup, state upload, compute/render passes, controls, and cleanup.
- `shaders/source/*.hlsl`: portable compute, vertex, and fragment shaders.
- `shaders/MSL/*.msl`: Metal source shaders included for macOS.
- `compile-shaders.sh`: optional offline compilation for other SDL GPU backends.
