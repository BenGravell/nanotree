# Usage

Run the desktop app from the repository root so it can find the `assets/` directory.

## Local app

### Run

```bash
build/release/nanotree
```

### Run debug

```bash
build/debug/nanotree
```

## Web app

### Run

```bash
cd ~/emsdk
source emsdk_env.sh
cd ~/nanotree

emrun dist/index.html
```

## How to use the app

Select an editing mode in the right sidebar, then click or drag in the environment
to move the start or goal, add obstacles, or erase obstacles. The grid toggle snaps
the brush to grid centers; the bin button clears obstacles.

Use the planner controls to run continuously, grow until the goal is reached,
pause, or advance one iteration. Reset clears the tree, and the shuffle toggle
enables rewiring. Carry controls node retention between iterations; Samples
controls how many samples are attempted per growth iteration.

The left sidebar shows goal status, path cost, node counts, and timings. Its
visibility toggles show or hide the tree, path, and obstacles.
