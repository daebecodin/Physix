# SDL3 GPU two-body example

This example draws two lit 3D spheres with SDL3 GPU. A compute shader advances both bodies with the same gravitational acceleration and kick–drift–kick leapfrog sequence used by `Physix::simulateSystem`. Both masses move. The rendering shader reads the updated GPU state directly; there is no per-frame CPU readback.

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

The Sun uses `Physix::Masses::SUN_MASS`; the Earth uses `Physix::Masses::EARTH_MASS` and `Physix::Distances::EARTH_ORBITAL_DISTANCE`. The starting relative speed comes from `Physix::Orbital::circularOrbitSpeed` with the combined mass. The Sun receives the opposite velocity scaled by the mass ratio, so the starting momentum is zero. The compute shader converts Physix's gravitational constant to solar masses, AU, and days and performs two 0.5-day steps per displayed frame. It calculates both bodies' new positions independently from one shared old state, then recalculates acceleration at the new positions before the second half kick. These steps run on the GPU.

The simulation uses 32-bit floats on the GPU. This is a visual demo, not a high-precision orbital integrator. The spheres' radii are greatly exaggerated and do not represent physical diameters. The camera is fixed above the orbital plane with perspective and depth testing. Resize the window to change its aspect ratio. A frame advances one simulated day, so playback speed depends on display frame rate; the numerical step remains 0.5 day.

## Files

- `two_body_gpu.cpp`: SDL setup, state upload, compute/render passes, controls, and cleanup.
- `shaders/source/*.hlsl`: portable compute, vertex, and fragment shaders.
- `shaders/MSL/*.msl`: Metal source shaders included for macOS.
- `compile-shaders.sh`: optional offline compilation for other SDL GPU backends.
