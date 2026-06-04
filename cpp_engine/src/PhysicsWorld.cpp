#include "physengine/PhysicsWorld.hpp"
#include "physengine/integrators/RK4Integrator.hpp"
#include <iostream>
#include <cmath>

void PhysicsWorld::addBody(std::unique_ptr<RigidBody> body) {
    bodies.push_back(std::move(body));
}

void PhysicsWorld::step() {
    /* 1. Clear accumulated forces from last tick */
    for (auto& body : bodies) {
        body->clearForces();
    }

    /* 2. Apply gravity to every non-static body */
    for (auto& body : bodies) {
        if (!body->isStatic) {
            body->applyForce(Vector3(0, -9.81 * body->mass, 0));
        }
    }

    /* 3. Integrate all bodies one timestep */
    for (auto& body : bodies) {
        RK4Integrator::integrate(*body, dt);
    }

    /* 4. Reflect bodies that escaped the world boundary */
    resolveCollisions();     // ← Collisions
    applyBoundaries();

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
    std::cout << "=== PhysicsWorld t=" << time << "s | bodies=" << bodies.size() << " ===\n";
    for (size_t i = 0; i < bodies.size(); ++i) {
        const auto& b = bodies[i];
        std::cout << "  Body " << i 
                  << " pos(" << b->position.x() << ", " << b->position.y() << ", " << b->position.z() << ")"
                  << " vel(" << b->velocity.x() << ", " << b->velocity.y() << ", " << b->velocity.z() << ")\n";
    }
}

/* ─────────────────────────────────────────────────────────────────
 * applyBoundaries
 *
 * For each non-static body, check all six faces of the bounding box.
 * When a body crosses a face:
 *   • push its position back inside (accounting for its radius)
 *   • flip the velocity component and scale by restitution
 *     (restitution 1.0 = perfectly elastic, 0.0 = no bounce)
 * ───────────────────────────────────────────────────────────────── */
void PhysicsWorld::applyBoundaries() {
    if (!bounds.enabled) return;

    for (auto& body : bodies) {
        if (body->isStatic) continue;

        const double r = body->radius;

        /* ── X axis ── */
        if (body->position.v.x - r < bounds.minX) {
            body->position.v.x  = bounds.minX + r;
            body->velocity.v.x  =  std::abs(body->velocity.v.x) * body->restitution;
        } else if (body->position.v.x + r > bounds.maxX) {
            body->position.v.x  = bounds.maxX - r;
            body->velocity.v.x  = -std::abs(body->velocity.v.x) * body->restitution;
        }

        /* ── Y axis ── */
        if (body->position.v.y - r < bounds.minY) {
            body->position.v.y  = bounds.minY + r;
            body->velocity.v.y  =  std::abs(body->velocity.v.y) * body->restitution;
        } else if (body->position.v.y + r > bounds.maxY) {
            body->position.v.y  = bounds.maxY - r;
            body->velocity.v.y  = -std::abs(body->velocity.v.y) * body->restitution;
        }

        /* ── Z axis (only matters when going 3-D) ── */
        if (body->position.v.z - r < bounds.minZ) {
            body->position.v.z  = bounds.minZ + r;
            body->velocity.v.z  =  std::abs(body->velocity.v.z) * body->restitution;
        } else if (body->position.v.z + r > bounds.maxZ) {
            body->position.v.z  = bounds.maxZ - r;
            body->velocity.v.z  = -std::abs(body->velocity.v.z) * body->restitution;
        }
    }
}

// ==================== COLLISION DETECTION ====================
void PhysicsWorld::resolveCollisions() {
    const size_t n = bodies.size();
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {        // This is fine, but let's be extra safe
            auto& a = bodies[i];
            auto& b = bodies[j];

            if (a->isStatic && b->isStatic) continue;

            Vector3 delta = a->position - b->position;
            double distSq = delta.dot(delta);
            double minDist = a->radius + b->radius;

            if (distSq >= minDist * minDist || distSq < 1e-12) continue;

            double dist = std::sqrt(distSq);
            Vector3 normal = delta * (1.0 / dist);

            // Position correction
            double overlap = minDist - dist;
            Vector3 correction = normal * (overlap * 0.5);

            if (!a->isStatic) a->position = a->position + correction;
            if (!b->isStatic) b->position = b->position - correction;

            // Velocity resolution
            Vector3 relVel = a->velocity - b->velocity;
            double velAlongNormal = relVel.dot(normal);

            if (velAlongNormal > 0) continue; // separating

            double e = std::min(a->restitution, b->restitution);
            double impulseScalar = -(1.0 + e) * velAlongNormal;
            impulseScalar /= (1.0 / a->mass + 1.0 / b->mass);

            Vector3 impulse = normal * impulseScalar;

            if (!a->isStatic) a->velocity = a->velocity + impulse * (1.0 / a->mass);
            if (!b->isStatic) b->velocity = b->velocity - impulse * (1.0 / b->mass);
        }
    }
}
