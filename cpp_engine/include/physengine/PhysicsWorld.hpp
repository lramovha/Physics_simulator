#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include "RigidBody.hpp"
#include "Vec3.hpp"
#include <vector>
#include <memory>
#include <iostream>

class PhysicsWorld {
public:
    std::vector<std::unique_ptr<RigidBody>> bodies;
    double time = 0.0;
    double dt = 0.016;   // ~60 FPS

    PhysicsWorld() = default;

    void addBody(std::unique_ptr<RigidBody> body);
    void step();
    void step(int numSteps);

    void clear();
    void printStatus() const;
};

#endif