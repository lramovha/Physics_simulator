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

* **C11** – Custom mathematics library and numerical methods
* **C++17** – Physics engine and native simulation systems
* **Java 26** – Desktop application and JNI integration
* **JavaFX 26.0.1** – Graphical interface and real-time visualization
* **JNI** – Java-to-C++ native communication
* **CMake** – Native build configuration
* **MinGW-w64 (UCRT64)** – C/C++ compilation toolchain
* **GCC / G++** – Native compilers
* **Catch2** – C/C++ testing
* **JUnit** – Java testing

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

* Windows 10/11
* MSYS2 with the UCRT64 environment
* CMake 3.16+
* MinGW-w64 GCC/G++ toolchain
* JDK 26.0.1
* JavaFX SDK 26.0.1

### Full Build and Run (Recommended)

The project includes an automated build script that handles the complete build and launch process.

The script performs the following operations:

Cleans previous build files.

Configures the native project using CMake and MinGW Makefiles.

Compiles the C mathematics library, C++ physics engine, and JNI bridge.

Runs the native test executables.

Executes the native demonstration program.

Compiles all Java application sources.

Copies JavaFX CSS resources into the compiled application.

Launches the JavaFX Physics Simulator.

From the project root, execute:

```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

### Build Configuration

The native engine is compiled using:

MinGW Makefiles

UCRT64 GCC and G++

Release configuration

Parallel compilation (-j4)


### Native Tests

The build script executes the following native test programs:

```bash
./build/bin/test_physmath
./build/bin/physengine_test
```

It also launches the native demonstration executable:

```bash
./build/bin/main_demo
```

### JavaFX Application

The Java application is compiled directly using javac, with the JavaFX modules:

```bash
javafx.controls
javafx.graphics
```

The application is launched through:

```bash
com.physics.sim.MainApp
```

The native library path is configured to load the compiled C++ engine through JNI.

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

**Actively developed – Physics Simulator Pro v2.0.**

The project currently includes a custom C mathematics library, a C++ physics engine, a generic RK4 solver, JNI integration, and a JavaFX desktop application.

The build process is automated through a dedicated Bash script that configures, compiles, tests, and launches the complete application.

Future improvements include more advanced collision detection, additional numerical integration methods, enhanced physics systems, and eventually 3D simulation and rendering.

---

## Author

**Lasy Ramovha**

[GitHub](https://github.com/lramovha) · [Portfolio](https://lasyramovha-portfolio-web.netlify.app) · [Credly](https://www.credly.com/users/lasy-ramovha)





