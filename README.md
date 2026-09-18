# Physics Simulator

A custom physics simulation engine built from the ground up using **C, C++, Java, JavaFX, and JNI**.

The project combines a custom mathematics library, a C++ physics engine, a hand-written **Runge-Kutta 4 (RK4)** ODE solver, and a JavaFX interface for real-time simulation.

![Physics Simulator](https://github.com/user-attachments/assets/bd5794d8-e059-4721-80ec-7d471de9394e)

---

## Features

* Custom C mathematics library
* 3D vector and matrix operations
* Generic RK4 ODE solver
* C++ physics engine
* Real-time simulation
* Gravity and collision handling
* Restitution-based bouncing
* Interactive object spawning
* Motion trails
* JavaFX visualization
* C++ ↔ Java communication through JNI
* Catch2 and JUnit tests

---

## Scenes

| Scene                 | Demonstrates                              |
| --------------------- | ----------------------------------------- |
| **Falling Balls**     | Gravity and boundary collisions           |
| **Projectile Motion** | Initial velocity and gravitational motion |
| **Bouncing Balls**    | Collision response and restitution        |
| **Simple Pendulum**   | Nonlinear ODEs solved with RK4            |
| **Double Pendulum**   | Coupled nonlinear ODEs and chaotic motion |

The GUI allows scenes to be switched while the simulation is running, with interactive gravity controls and object spawning.

---

## Architecture

```text
Java / JavaFX
      │
      │ JNI
      ▼
C++ Physics Engine
      │
      ▼
C Mathematics Core
```

### C Math Core

Provides vectors, matrices, calculus utilities, and the generic RK4 solver.

### C++ Physics Engine

Handles the physics world, rigid bodies, integration, collisions, and individual simulation scenes.

### JNI Bridge

Connects the native C++ engine to the Java application.

### Java / JavaFX

Handles rendering, controls, animation, and user interaction.

---

## Technology

* **C11**
* **C++17**
* **Java 17**
* **JavaFX 21**
* **JNI**
* **CMake**
* **Gradle**
* **Catch2**
* **JUnit**

---

## Project Structure

```text
c_core/       C mathematics library and RK4 solver
cpp_engine/   C++ physics engine and JNI bridge
java_app/     JavaFX desktop application
tests/        C and C++/Java tests
```

---

## Building

### Requirements

* CMake 3.16+
* GCC/MinGW or another C/C++ compiler
* JDK 17+
* JavaFX 21

### Build the native engine

```bash
cmake -B build -S .
cmake --build build --config Release -j4
```

### Run tests

```bash
./build/bin/test_physmath
./build/bin/physengine_test
```

### Run the JavaFX application

```bash
cd java_app
./gradlew run
```

On Windows:

```bash
gradlew.bat run
```

---

## Screenshots

### Current Version

![Current Physics Simulator](https://github.com/user-attachments/assets/1cea2748-0991-46d2-8bb0-f40450201e49)

![Current Physics Simulator](https://github.com/user-attachments/assets/a84bc86b-88fb-4c2c-aa3a-213cd86f3e09)

![Current Physics Simulator](https://github.com/user-attachments/assets/d0e5146f-d765-405f-a2db-d8eb5e2c3856)

![Current Physics Simulator](https://github.com/user-attachments/assets/9541b2a7-46ab-4f98-b769-ce5b731ab1f4)

### Original Version

![Original Physics Simulator](https://github.com/user-attachments/assets/dd54c204-9ba6-4502-9941-3948192afafb)

---

## Status

**Actively developed.**

Future improvements include more advanced collision detection, additional numerical integrators, improved physics systems, and eventually 3D simulation and rendering.

---

## Author

**Lasy Ramovha**

[GitHub](https://github.com/lramovha) · [Portfolio](https://lasyramovha-portfolio-web.netlify.app) · [Credly](https://www.credly.com/users/lasy-ramovha)





