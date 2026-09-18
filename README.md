# Physics Simulator

A physics simulation engine built from the ground up using **C, C++, Java, JavaFX, and JNI**.

The project combines a custom mathematical core, a C++ physics engine, numerical integration using a hand-written **4th-order Runge-Kutta (RK4)** solver, and a JavaFX desktop application for real-time visualization.

The simulator currently includes five interactive scenes, ranging from basic gravitational motion to nonlinear chaotic systems such as the **double pendulum**.

---

## Overview

**Physics Simulator** is a multi-language physics engine designed to explore the relationship between:

* Numerical methods
* Classical mechanics
* Linear algebra
* Ordinary differential equations
* Systems programming
* Object-oriented design
* Native/managed interoperability
* Real-time simulation
* Software architecture

Rather than relying on an existing physics engine, the core simulation and numerical components were implemented from scratch.

The project is structured so that computationally intensive physics calculations are handled by native C/C++ code, while JavaFX provides the higher-level graphical interface.

---

## Key Features

* Custom C mathematics library
* 3D vector and matrix operations
* Generic Runge-Kutta 4th-order ODE solver
* C++ rigid-body physics engine
* Real-time physics simulation
* Gravity-based motion
* Boundary collision handling
* Restitution-based bouncing
* Projectile motion
* Simple pendulum simulation
* Nonlinear double pendulum simulation
* Chaotic motion and sensitivity to initial conditions
* Interactive gravity control
* Live object spawning
* Motion trails
* JavaFX real-time rendering
* Native C++ ↔ Java communication through JNI
* CMake-based native build system
* Gradle-based Java application
* Automated unit testing with Catch2/JUnit
* Cross-language project architecture

---

# Demonstration

The simulator provides several different environments for experimenting with numerical integration and classical mechanics.

### Falling Balls

Demonstrates gravitational acceleration, numerical integration, and interaction with the simulation boundaries.

### Projectile Motion

Demonstrates initial velocity and gravitational acceleration producing projectile trajectories.

### Bouncing Balls

Demonstrates collision response using restitution and allows additional balls to be spawned while the simulation is running.

### Simple Pendulum

Models a nonlinear pendulum using an ordinary differential equation:

```text
θ' = ω
ω' = -(g/L)sin(θ)
```

The system is integrated numerically using RK4.

### Double Pendulum

The double pendulum uses a coupled nonlinear system of differential equations.

Because of its sensitivity to initial conditions, small changes in the starting state can produce significantly different trajectories over time.

This makes it a useful demonstration of numerical simulation and chaotic dynamics.

---

# Architecture

The project is divided into four primary layers.

```text
┌─────────────────────────────────────────────┐
│              Java / JavaFX GUI              │
│                                             │
│ MainApp.java                                │
│ Scene selection, rendering, controls,       │
│ gravity slider, object spawning, trails     │
└──────────────────────┬──────────────────────┘
                       │
                       │ JNI
                       ▼
┌─────────────────────────────────────────────┐
│                 JNI Bridge                  │
│                                             │
│ PhysicsJNI.cpp                              │
│ Converts Java calls into native C++ calls  │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             C++ Physics Engine              │
│                                             │
│ PhysicsWorld                                │
│ RigidBody                                   │
│ RK4Integrator                               │
│ Scene implementations                       │
│ Collision and boundary handling             │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│                 C Math Core                 │
│                                             │
│ Vec3                                        │
│ Mat3                                        │
│ Calculus utilities                          │
│ Generic RK4 ODE solver                      │
└─────────────────────────────────────────────┘
```

## Why this architecture?

The project deliberately separates responsibilities between languages.

### C

The C layer contains low-level mathematical functionality.

It provides:

* Vector operations
* Matrix operations
* Mathematical utilities
* Calculus functionality
* Generic RK4 numerical integration

The goal is to keep the mathematical foundation independent of the higher-level simulation code.

### C++

The C++ layer builds the actual physics engine on top of the mathematical core.

It is responsible for:

* Physics worlds
* Rigid bodies
* Scene management
* Forces
* Integration
* Collision handling
* Simulation state
* Per-scene behaviour

C++ provides the object-oriented structure required to manage the increasingly complex simulation.

### JNI

The JNI layer acts as the boundary between native C++ and Java.

The Java application can request operations such as:

```text
createScene()
step()
getBodyPositions()
getPendulumState()
setGravity()
spawnBall()
```

JNI converts these calls into operations performed by the native physics engine.

This allows the computational core to remain native while Java handles the user interface.

### Java / JavaFX

The JavaFX application provides the visual layer.

It handles:

* Rendering
* Scene selection
* User input
* Gravity controls
* Animation timing
* Object spawning
* Motion trails
* Displaying simulation state

The GUI does not perform the core physics calculations itself.

---

# Scenes

| Scene                 | Description                                                                       | Main Concepts                           |
| --------------------- | --------------------------------------------------------------------------------- | --------------------------------------- |
| **Falling Balls**     | Balls fall under gravity and interact with the world boundaries.                  | Gravity, integration, collision         |
| **Projectile Motion** | Objects are launched with an initial velocity and follow a projectile trajectory. | Kinematics, gravity                     |
| **Bouncing Balls**    | Multiple balls bounce using restitution-based collision response.                 | Collision response, restitution         |
| **Simple Pendulum**   | A nonlinear pendulum is solved using RK4.                                         | ODEs, RK4, numerical methods            |
| **Double Pendulum**   | A coupled nonlinear system produces complex and chaotic motion.                   | Coupled ODEs, nonlinear dynamics, chaos |

---

# Numerical Methods

A major purpose of the project is to implement and apply numerical methods rather than relying exclusively on analytical solutions.

## Runge-Kutta 4th Order

The simulator uses the classical fourth-order Runge-Kutta method to integrate differential equations.

For a system:

```text
dy/dt = f(t, y)
```

the RK4 method calculates:

```text
k1 = f(t, y)

k2 = f(t + h/2, y + hk1/2)

k3 = f(t + h/2, y + hk2/2)

k4 = f(t + h, y + hk3)
```

and updates the state using:

```text
y(next) = y + h/6 (k1 + 2k2 + 2k3 + k4)
```

where:

* `y` is the current state
* `h` is the timestep
* `f` represents the differential equation
* `k1` through `k4` are intermediate estimates

The implementation is designed as a reusable ODE solver so that different physical systems can use the same numerical integration infrastructure.

---

# Pendulum Simulation

The simple pendulum is represented using the state variables:

```text
θ = angular position
ω = angular velocity
```

with:

```text
θ' = ω

ω' = -(g/L)sin(θ)
```

This system is integrated numerically at every simulation step.

The double pendulum extends this concept into a coupled nonlinear system where the acceleration of each pendulum depends on the state of both pendulum segments.

This makes the double pendulum particularly useful for demonstrating how relatively simple physical equations can produce complex behaviour.

---

# Physics Engine

The C++ engine provides the main simulation infrastructure.

A simplified simulation loop is:

```text
User / JavaFX
      │
      ▼
JNI step()
      │
      ▼
PhysicsWorld
      │
      ├── Apply forces
      │
      ├── Integrate state
      │
      ├── Detect boundaries
      │
      ├── Resolve collisions
      │
      └── Update simulation
      │
      ▼
Updated state
      │
      ▼
JNI
      │
      ▼
JavaFX rendering
```

The simulation is stepped continuously while the JavaFX application renders the current state.

---

# User Interface

The JavaFX application provides interactive controls for experimenting with the simulation.

Current interface functionality includes:

* Scene selection
* Gravity adjustment
* Ball/object spawning
* Real-time rendering
* Motion trails
* Simulation state visualization
* Pendulum visualization
* Scene-specific controls

The interface is intentionally separated from the native physics implementation so that changes to the rendering layer do not require rewriting the simulation engine.

---

# Technology Stack

## Languages

* **C11**
* **C++17**
* **Java 17**

## Frameworks and Libraries

* **JavaFX 21**
* **JNI**
* **CMake**
* **Gradle**
* **Catch2**
* **JUnit**

## Development Tools

* GCC / MinGW
* CMake
* Gradle
* Git
* Java Development Kit

---

# Project Structure

```text
PhysicsSimulator/
│
├── c_core/
│   ├── physmath/
│   │   ├── vector mathematics
│   │   ├── matrix mathematics
│   │   ├── calculus utilities
│   │   └── RK4 implementation
│   │
│   └── ...
│
├── cpp_engine/
│   ├── physics engine
│   ├── PhysicsWorld
│   ├── RigidBody
│   ├── RK4Integrator
│   ├── scene implementations
│   ├── JNI bridge
│   └── ...
│
├── java_app/
│   ├── JavaFX application
│   ├── MainApp.java
│   ├── NativePhysicsWorld.java
│   ├── Gradle configuration
│   └── ...
│
├── tests/
│   ├── C math tests
│   ├── C++ physics tests
│   └── Java tests
│
├── CMakeLists.txt
├── build_run_commands.txt
└── README.md
```

---

# Building and Running

## Prerequisites

You will need:

* CMake 3.16+
* A C/C++ compiler

  * GCC/MinGW on Windows
  * GCC or Clang on Linux/macOS
* JDK 17+
* JavaFX 21

The Java application can obtain JavaFX through its Gradle configuration, so a manually installed JavaFX SDK is not required when using the Gradle workflow.

---

## 1. Clone the repository

```bash
git clone https://github.com/lramovha/Physics_simulator.git
cd Physics_simulator
```

---

## 2. Build the native engine

From the project root:

```bash
cmake -B build -S .
cmake --build build --config Release -j4
```

This builds the native components:

```text
C math core
      ↓
C++ physics engine
      ↓
JNI bridge
      ↓
Native shared library
```

Depending on the operating system, the resulting library will be similar to:

```text
physengine.dll
libphysengine.so
libphysengine.dylib
```

---

## 3. Run the native tests

After building:

```bash
./build/bin/test_physmath
```

This runs the C mathematics tests.

Run the C++ physics tests with:

```bash
./build/bin/physengine_test
```

---

## 4. Run the console demo

The project also contains a native console demonstration:

```bash
./build/bin/main_demo
```

This allows parts of the physics engine to be tested without launching the JavaFX interface.

---

## 5. Run the JavaFX application

Move into the Java application directory:

```bash
cd java_app
```

Then run:

```bash
./gradlew run
```

On Windows:

```bash
gradlew.bat run
```

The Gradle configuration handles the JavaFX dependencies and native library setup required by the application.

---

# Native Library Loading

The Java application communicates with the C++ engine through JNI.

The native library must therefore be available to the JVM at runtime.

The Gradle build includes native-library handling so that the compiled native library can be copied into the appropriate application build directory.

The overall dependency chain is:

```text
JavaFX Application
       │
       ▼
NativePhysicsWorld.java
       │
       ▼
JNI
       │
       ▼
PhysicsJNI.cpp
       │
       ▼
C++ Physics Engine
       │
       ▼
C Math Core
```

---

# Testing

Testing is included at multiple levels of the project.

## C Math Core

The mathematical layer is tested independently to verify functionality such as:

* Vector operations
* Matrix operations
* Numerical calculations
* RK4 integration

## C++ Physics Engine

The physics layer contains tests for core simulation behaviour, including physics-world functionality and state updates.

## Java

JUnit is used for Java-side testing where appropriate.

The multi-layer testing approach helps identify whether a problem originates in:

```text
Mathematics
    ↓
Physics Engine
    ↓
JNI
    ↓
Java Application
```

rather than relying exclusively on visual inspection of the simulation.

---

# Design Goals

The project was built around several design goals.

### 1. Build the mathematics from the ground up

Instead of depending entirely on an existing mathematics or physics library, the project implements the core numerical functionality itself.

### 2. Separate simulation from rendering

The physics engine should be able to operate independently of JavaFX.

This makes it possible to test the native engine without launching the GUI.

### 3. Explore cross-language interoperability

JNI provides practical experience communicating between native C++ code and Java.

### 4. Apply numerical methods to real systems

The RK4 implementation is not just an isolated numerical-method exercise. It is used directly to simulate physical systems.

### 5. Build toward a reusable engine

The architecture is designed so that additional scenes and physical systems can be added without fundamentally changing the entire application.

---

# What I Learned

This project provided practical experience with several areas of computer science and software engineering.

### Systems Programming

Working directly with C and C++ provided experience with:

* Memory management
* Headers and compilation units
* Static and shared libraries
* Native builds
* CMake
* C/C++ interoperability

### Numerical Computing

Implementing RK4 and applying it to physical systems provided practical experience with:

* Ordinary differential equations
* Numerical integration
* Timesteps
* State vectors
* Numerical approximation
* Stability considerations

### Object-Oriented Design

The C++ engine uses classes to represent simulation concepts such as:

```text
PhysicsWorld
RigidBody
Scene
Integrator
```

This provides a structured foundation for expanding the engine.

### Cross-Language Development

JNI introduced the additional challenge of connecting:

```text
Java
  ↕
JNI
  ↕
C++
  ↕
C
```

This required understanding both sides of the interface and carefully managing data passed between native and managed environments.

### Real-Time Applications

The project also provided experience with:

* Animation loops
* Frame updates
* Rendering
* User input
* Simulation timesteps
* Synchronizing computation and visualization

---

# Current Limitations

The project is still under active development.

Some current limitations include:

* General collision detection is still relatively limited
* The current engine focuses primarily on simple rigid-body scenarios
* The renderer is currently 2D
* The JNI interface could be expanded
* Cross-platform packaging still requires additional work
* The physics engine does not yet provide the feature set of mature production physics engines

These limitations are intentional opportunities for future development rather than dependencies on an external physics engine.

---

# Future Development

Planned and possible improvements include:

## 3D Physics

Extend the engine from primarily 2D visualizations toward a full 3D simulation environment.

Potential additions:

* 3D rigid bodies
* Quaternion orientation
* 3D collision detection
* 3D rendering
* Camera controls

## Improved Collision System

Expand collision handling beyond simple boundaries.

Potential collision shapes include:

```text
Sphere
Box
Capsule
Plane
```

with increasingly sophisticated collision detection and response.

## Spatial Partitioning

Introduce structures such as:

* Uniform grids
* Quadtrees
* Octrees
* Bounding volume hierarchies

to reduce unnecessary collision checks.

## More Numerical Integrators

Add additional integration methods for comparison:

* Euler
* Semi-Implicit Euler
* RK2
* RK4
* Verlet
* Velocity Verlet

This would allow the simulator to compare accuracy and stability between integration techniques.

## Physics Diagnostics

Add tools for displaying:

* Velocity
* Acceleration
* Energy
* Momentum
* Simulation timestep
* Frame rate
* Numerical error

## Visualization

Additional visualization features could include:

* Vector fields
* Force arrows
* Velocity vectors
* Trajectory graphs
* Energy graphs
* Numerical error plots

## Cross-Platform Packaging

Improve the build and packaging system for:

* Windows
* Linux
* macOS

including automated native-library packaging.

---

# Screenshots

## Falling Balls / Physics World

<img width="1207" height="926" alt="Physics Simulator" src="https://github.com/user-attachments/assets/dd54c204-9ba6-4502-9941-3948192afafb" />

## Projectile Motion

<img width="1917" height="1017" alt="Projectile Motion" src="https://github.com/user-attachments/assets/bd5794d8-e059-4721-80ec-7d471de9394e" />

## Bouncing Balls

<img width="1912" height="1021" alt="Bouncing Balls" src="https://github.com/user-attachments/assets/1cea2748-0991-46d2-8bb0-f40450201e49" />

## Simple Pendulum

<img width="1917" height="1016" alt="Simple Pendulum" src="https://github.com/user-attachments/assets/a84bc86b-88fb-4c2c-aa3a-213cd86f3e09" />

## Double Pendulum

<img width="1917" height="1020" alt="Double Pendulum" src="https://github.com/user-attachments/assets/d0e5146f-d765-405f-a2db-d8eb5e2c3856" />

## Interactive Simulation

<img width="1917" height="1020" alt="Physics Simulator Interactive Scene" src="https://github.com/user-attachments/assets/9541b2a7-46ab-4f98-b769-ce5b731ab1f4" />

---

# Project Status

**Active development**

The current version focuses on establishing the underlying mathematical, physics, and cross-language architecture.

The project will continue to evolve toward a more complete physics and simulation framework, with particular focus on improved collision detection, 3D simulation, numerical analysis tools, and engine architecture.

---

# Why This Project Matters

This project is more than a graphical demonstration of physics.

It combines several areas of computer science into a single application:

```text
Mathematics
     │
     ▼
Numerical Methods
     │
     ▼
C Programming
     │
     ▼
C++ Physics Engine
     │
     ▼
JNI Interoperability
     │
     ▼
Java Application
     │
     ▼
Real-Time Visualization
```

The result is a complete pipeline where mathematical equations are transformed into numerical algorithms, numerical algorithms drive a native physics engine, and the resulting simulation state is displayed interactively through a JavaFX application.

---

# Author

**Lasy Ramovha**

Computer Science Student & Software Developer

* GitHub: https://github.com/lramovha
* Portfolio: https://lasyramovha-portfolio-web.netlify.app
* Credly: https://www.credly.com/users/lasy-ramovha

---

## License

This project is currently intended as a personal learning and development project.




