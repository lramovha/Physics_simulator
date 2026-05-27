#include "physengine/PhysicsWorld.hpp"
#include <iostream>

int main() {
    std::cout << "=== Physics Engine Test ===\n";
    std::cout << "PhysEngine Version: " << PHYSENGINE_VERSION << "\n\n";

    PhysicsWorld world;
    world.dt = 0.01;   // small timestep

    // Falling ball
    auto ball = std::make_unique<RigidBody>(1.0, Vector3(0, 10.0, 0));
    ball->isStatic = false;
    world.addBody(std::move(ball));

    // Static ground
    auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, 0.0, 0));
    ground->isStatic = true;
    world.addBody(std::move(ground));

    std::cout << "Initial state:\n";
    world.printStatus();

    std::cout << "\nRunning simulation for 300 steps...\n";

    for (int i = 0; i < 300; ++i) {
        world.step();
        if (i % 50 == 0 || i == 299) {
            world.printStatus();
        }
    }

    std::cout << "\n=== Simulation Finished ===\n";
    return 0;
}