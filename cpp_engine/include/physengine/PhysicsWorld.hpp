#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#ifndef PHYSENGINE_VERSION
#define PHYSENGINE_VERSION "0.8.0"
#endif

#include "RigidBody.hpp"
#include "Vec3.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include "SpatialHash.hpp"   

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

    // Controllable physics parameters
    double gravity       = 9.81;
    double airResistance = 0.08;   // linear drag coefficient
    double elasticity    = 0.78;   // default restitution applied to new/all bodies
    double friction      = 0.3;    // tangential damping on boundary contact (0–1)

    WorldBounds bounds;             // boundary box (off by default)

    // Stats
    int collisionCount = 0;

    PhysicsWorld() = default;

    void addBody(std::unique_ptr<RigidBody> body);

    /* Advance one timestep: apply gravity → integrate → clamp bounds */
    void step();

    /* Advance N timesteps */
    void step(int numSteps);

    void clear();
    void printStatus() const;

    void setGravity(double g);
    void setAirResistance(double k);
    void setElasticity(double e);   // updates all existing bodies
    void setFriction(double f);

    // Energy helpers
    double getKineticEnergy() const;
    double getPotentialEnergy() const;
    double getTotalEnergy() const;

private:
    /* Reflect any body that has crossed the world boundary */
    void applyBoundaries();
    void resolveCollisions();   // to handle rigid body collisions

    SpatialHash spatialHash;
};

#endif /* PHYSICS_WORLD_HPP */