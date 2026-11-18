# Orbit demo commands

Run these commands from the Physix project root.

## Build the demo

```sh
cmake -S . -B build
cmake --build build --target orbit
```

## View all graphs interactively

```sh
./build/orbit
```

The demo prints the benchmark timings, then shows the graphs in this order:

1. Two-body orbit
2. Relative energy error
3. Relative angular momentum error
4. Eccentricity

## View only the energy graph

```sh
./build/orbit --energy
```

## Export all four graphs as PNGs

```sh
./build/orbit chapter4
```

This writes `chapter4-orbit.png`, `chapter4-energy.png`, `chapter4-angular-momentum.png`, and `chapter4-eccentricity.png` in the current directory.
