# Physics Simulator

A physics engine built from the ground up across three languages: a **C** math core, a **C++** rigid-body engine, and a **Java/JavaFX** desktop GUI connected through a hand-written **JNI** bridge. Five interactive scenes — including a chaotic double pendulum — run in real time, driven by a custom Runge-Kutta (RK4) ODE solver.

<!-- 
TODO: Add a screenshot or GIF here once recorded, e.g.:
![Physics Simulator demo](docs/demo.gif)
-->

## Why this project exists

Most "physics simulation" student projects either lean entirely on a game engine (Unity/Unreal) or stay in a single language. This one is an exercise in building the actual numerical and architectural plumbing by hand: writing the ODE solver myself, designing a rigid-body world with collision and boundary handling in C++, and then exposing that native simulation to a managed GUI layer through JNI — the same pattern real engines use when a performance-critical core needs to talk to a higher-level application layer.

## Architecture

```
┌───────────────────────── ┐
│   Java / JavaFX GUI      │  MainApp.java — canvas rendering, scene selector,
│   (java_app/)            │  gravity slider, live ball spawning, motion trails
└────────────┬─────────────┘
             │ JNI calls (NativePhysicsWorld.java)
┌────────────┴─────────────┐
│   JNI Bridge             │  PhysicsJNI.cpp — exposes createScene(), step(),
│   (cpp_engine/jni/)      │  getBodyPositions(), getPendulumState(), etc.
└────────────┬─────────────┘
┌────────────┴─────────────┐
│   C++ Physics Engine     │  PhysicsWorld, RigidBody, RK4Integrator,
│   (cpp_engine/)          │  5 Scene classes (Falling Balls, Projectile,
│                          │  Bouncing Balls, Pendulum, Double Pendulum)
└────────────┬─────────────┘
┌────────────┴─────────────┐
│   C Math Core            │  Vec3, Mat3, calculus helpers, and a generic
│   (c_core/)              │  rk4_step() ODE solver used by every scene
└──────────────────────────┘
```

## Scenes

| Scene | What it demonstrates |
|---|---|
| **Falling Balls** | Basic gravity integration and world-boundary collision |
| **Projectile Motion** | Initial-velocity kinematics under gravity |
| **Bouncing Balls** | Restitution-based collision response, live ball spawning |
| **Simple Pendulum** | Direct ODE modelling (θ′ = ω, ω′ = −(g/L)sin θ) solved via RK4 |
| **Double Pendulum** | Coupled nonlinear ODEs producing chaotic motion — sensitive to initial conditions |

The GUI lets you switch scenes, adjust gravity with a slider, and spawn new bodies while the simulation runs.

## Tech stack

- **C** — vector/matrix math, calculus utilities, a generic 4th-order Runge-Kutta ODE solver
- **C++17** — `PhysicsWorld`, `RigidBody`, per-scene physics logic, RK4 integration wrapper
- **JNI** — native bridge exposing the C++ simulation to Java
- **Java 17 / JavaFX 21** — desktop GUI: canvas rendering, controls, animation loop
- **CMake** — builds the C core and C++ engine into a shared library consumed by the JVM
- **Catch2 / JUnit** — unit tests for the math core and the physics world

## Building and running

### Prerequisites

- CMake 3.16+
- A C/C++ compiler (GCC/MinGW on Windows, GCC/Clang on Linux/macOS)
- JDK 17+
- [JavaFX SDK 21](https://gluonhq.com/products/javafx/) (or use the Gradle build in `java_app/`, which pulls JavaFX via Maven Central automatically)

### 1. Build the native engine (C core + C++ engine + JNI bridge)

```bash
cmake -B build -S .
cmake --build build --config Release -j4
```

This produces the shared library (`physengine.dll` / `libphysengine.so` / `libphysengine.dylib` depending on platform) in `build/lib/`, plus test binaries and a console demo in `build/bin/`.

### 2. Run the native test suite and console demo

```bash
./build/bin/test_physmath       # C math core unit tests
./build/bin/physengine_test     # C++ physics world tests
./build/bin/main_demo           # console-only scene demo, no GUI
```

### 3. Build and run the JavaFX GUI

The `java_app/` module uses Gradle and pulls JavaFX automatically:

```bash
cd java_app
./gradlew run
```

The native library built in step 1 must be discoverable at runtime — `copyNativeLibs` in `build.gradle` copies it into `build/libs/native` automatically when you build with Gradle from a clean checkout.

> If you prefer building without Gradle, see the manual `javac`/`java` invocation with `--module-path` to a local JavaFX SDK in [`build_run_commands.txt`](build_run_commands.txt).

## Project structure

```
c_core/         C math library (vectors, matrices, calculus, RK4 ODE solver)
cpp_engine/     C++ physics engine, scenes, and the JNI bridge
java_app/       JavaFX desktop GUI and the Java-side native bridge class
tests/          Catch2 unit tests for c_core and cpp_engine
```

## Status

Actively developed as a personal project to deepen understanding of numerical methods, systems-level C/C++, and cross-language interoperability. Possible next steps: 3D rendering, a broader collision system (currently boundary-only for general bodies), and packaging the JNI build for Linux/macOS alongside Windows.

## Author

Lasy Ramovha — [GitHub](https://github.com/lramovha) · [Portfolio](https://lasyramovha-portfolio-web.netlify.app)
