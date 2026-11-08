#include "physengine/PhysicsWorld.hpp"
#include "physengine/integrators/RK4Integrator.hpp"
#include <iostream>
#include <cmath>
#include <omp.h>

void PhysicsWorld::addBody(std::unique_ptr<RigidBody> body) {
    // Apply current global elasticity to new bodies
    if (!body->isStatic) {
        body->restitution = elasticity;
    }
    bodies.push_back(std::move(body));
}

void PhysicsWorld::setGravity(double g) {
    gravity = g;
}

void PhysicsWorld::setAirResistance(double k) {
    airResistance = k;
}

void PhysicsWorld::setElasticity(double e) {
    elasticity = e;
    for (auto& body : bodies) {
        if (!body->isStatic) {
            body->restitution = e;
        }
    }
}

void PhysicsWorld::setFriction(double f) {
    friction = f;
}

double PhysicsWorld::getKineticEnergy() const {
    double ke = 0.0;
    for (const auto& body : bodies) {
        if (body->isStatic) continue;
        double speedSq = body->velocity.dot(body->velocity);
        ke += 0.5 * body->mass * speedSq;
    }
    return ke;
}

double PhysicsWorld::getPotentialEnergy() const {
    // Reference height = bounds.minY (or 0 if bounds disabled)
    double y0 = bounds.enabled ? bounds.minY : 0.0;
    double pe = 0.0;
    for (const auto& body : bodies) {
        if (body->isStatic) continue;
        pe += body->mass * gravity * (body->position.y() - y0);
    }
    return pe;
}

double PhysicsWorld::getTotalEnergy() const {
    return getKineticEnergy() + getPotentialEnergy();
}

void PhysicsWorld::step() {
    const size_t n = bodies.size();

    // 1 + 2. Clear forces, apply gravity + air resistance
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        auto& body = bodies[i];
        body->clearForces();

        if (!body->isStatic) {
            // Gravity
            body->applyForce(Vector3(0, -gravity * body->mass, 0));

            // Linear air resistance: F = -k * v
            if (airResistance > 1e-9) {
                Vector3 drag = body->velocity * (-airResistance);
                body->applyForce(drag);
            }
        }
    }

    // 3. Integrate
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        if (!bodies[i]->isStatic) {
            RK4Integrator::integrate(*bodies[i], dt);
        }
    }

    // 4. Collisions + boundaries
    resolveCollisions();
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
    collisionCount = 0;
}

void PhysicsWorld::printStatus() const {
    std::cout << "=== PhysicsWorld t=" << time << "s | bodies=" << bodies.size()
              << " | KE=" << getKineticEnergy()
              << " | PE=" << getPotentialEnergy() << " ===\n";
}

void PhysicsWorld::applyBoundaries() {
    if (!bounds.enabled) return;

    for (auto& body : bodies) {
        if (body->isStatic) continue;

        const double r = body->radius;
        bool hit = false;

        /* ── X axis ── */
        if (body->position.v.x - r < bounds.minX) {
            body->position.v.x = bounds.minX + r;
            body->velocity.v.x =  std::abs(body->velocity.v.x) * body->restitution;
            // Friction damps tangential (Y) velocity
            body->velocity.v.y *= (1.0 - friction);
            hit = true;
        } else if (body->position.v.x + r > bounds.maxX) {
            body->position.v.x = bounds.maxX - r;
            body->velocity.v.x = -std::abs(body->velocity.v.x) * body->restitution;
            body->velocity.v.y *= (1.0 - friction);
            hit = true;
        }

        /* ── Y axis ── */
        if (body->position.v.y - r < bounds.minY) {
            body->position.v.y = bounds.minY + r;
            body->velocity.v.y =  std::abs(body->velocity.v.y) * body->restitution;
            body->velocity.v.x *= (1.0 - friction);
            hit = true;
        } else if (body->position.v.y + r > bounds.maxY) {
            body->position.v.y = bounds.maxY - r;
            body->velocity.v.y = -std::abs(body->velocity.v.y) * body->restitution;
            body->velocity.v.x *= (1.0 - friction);
            hit = true;
        }

        /* ── Z axis ── */
        if (body->position.v.z - r < bounds.minZ) {
            body->position.v.z = bounds.minZ + r;
            body->velocity.v.z =  std::abs(body->velocity.v.z) * body->restitution;
            hit = true;
        } else if (body->position.v.z + r > bounds.maxZ) {
            body->position.v.z = bounds.maxZ - r;
            body->velocity.v.z = -std::abs(body->velocity.v.z) * body->restitution;
            hit = true;
        }

        if (hit) {
            // Counted per body per frame; approximate
            #pragma omp atomic
            collisionCount++;
        }
    }
}

void PhysicsWorld::resolveCollisions() {
    spatialHash.clear();

    for (auto& body : bodies) {
        if (!body->isStatic) {
            spatialHash.insert(body.get());
        }
    }

    auto potentialPairs = spatialHash.getPotentialPairs();

    #pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < potentialPairs.size(); ++i) {
        auto& pair = potentialPairs[i];
        RigidBody* a = pair.first;
        RigidBody* b = pair.second;

        if (a->isStatic && b->isStatic) continue;

        Vector3 delta = a->position - b->position;
        double distSq = delta.dot(delta);
        double minDist = a->radius + b->radius;

        if (distSq >= minDist * minDist || distSq < 1e-12) continue;

        double dist = std::sqrt(distSq);
        Vector3 normal = delta * (1.0 / dist);

        double overlap = minDist - dist;
        Vector3 correction = normal * (overlap * 0.5);

        if (!a->isStatic) a->position = a->position + correction;
        if (!b->isStatic) b->position = b->position - correction;

        Vector3 relVel = a->velocity - b->velocity;
        double velAlongNormal = relVel.dot(normal);

        if (velAlongNormal > 0) continue;

        double e = std::min(a->restitution, b->restitution);
        double j = -(1.0 + e) * velAlongNormal;
        j /= (1.0 / a->mass + 1.0 / b->mass);

        Vector3 impulse = normal * j;

        if (!a->isStatic) a->velocity = a->velocity + impulse * (1.0 / a->mass);
        if (!b->isStatic) b->velocity = b->velocity - impulse * (1.0 / b->mass);

        #pragma omp atomic
        collisionCount++;
    }
}