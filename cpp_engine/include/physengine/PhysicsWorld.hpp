#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#ifndef PHYSENGINE_VERSION
#define PHYSENGINE_VERSION "0.6.0"
#endif

#include "RigidBody.hpp"
#include "Vec3.hpp"
#include <vector>
#include <memory>
#include <iostream>

/* ─────────────────────────────────────────────────────────────────
 * WorldBounds
 * Axis-aligned box that constrains all non-static RigidBodies.
 * Set enabled = true in a scene's init() to activate.
 * ───────────────────────────────────────────────────────────────── */
struct WorldBounds {
    double minX = -20.0;
    double maxX =  20.0;
    double minY = -10.0;
    double maxY =  30.0;
    double minZ = -20.0;
    double maxZ =  20.0;
    bool   enabled = false;
};

class PhysicsWorld {
public:
    std::vector<std::unique_ptr<RigidBody>> bodies;
    double      time   = 0.0;
    double      dt     = 0.016;
    WorldBounds bounds;             // boundary box (off by default)

    PhysicsWorld() = default;

    void addBody(std::unique_ptr<RigidBody> body);

    /* Advance one timestep: apply gravity → integrate → clamp bounds */
    void step();

    /* Advance N timesteps */
    void step(int numSteps);

    void clear();
    void printStatus() const;

private:
    /* Reflect any body that has crossed the world boundary */
    void applyBoundaries();
    void resolveCollisions();   // to handle rigid body collisions
};

#endif /* PHYSICS_WORLD_HPP */