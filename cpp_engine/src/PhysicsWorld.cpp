#include "physengine/PhysicsWorld.hpp"
#include "physengine/integrators/RK4Integrator.hpp"
#include <iostream>

void PhysicsWorld::addBody(std::unique_ptr<RigidBody> body) {
    bodies.push_back(std::move(body));
}

void PhysicsWorld::step() {
    for (auto& body : bodies) {
        body->clearForces();
        if (!body->isStatic) {
            body->applyForce(Vector3(0, -9.81 * body->mass, 0));
        }
    }

    for (auto& body : bodies) {
        RK4Integrator::integrate(*body, dt);
    }

    time += dt;
}

void PhysicsWorld::step(int numSteps) {
    for (int i = 0; i < numSteps; i++) {
        step();
    }
}

void PhysicsWorld::clear() {
    bodies.clear();
    time = 0.0;
}

void PhysicsWorld::printStatus() const {
    std::cout << "=== PhysicsWorld Status ===\n";
    std::cout << "Time: " << time << " s | Bodies: " << bodies.size() << "\n";
    for (size_t i = 0; i < bodies.size(); ++i) {
        const auto& b = bodies[i];
        std::cout << "Body " << i 
                  << ": pos(" << b->position.x() << ", " << b->position.y() << ", " << b->position.z() 
                  << ") vel(" << b->velocity.x() << ", " << b->velocity.y() << ", " << b->velocity.z() 
                  << ")\n";
    }
}