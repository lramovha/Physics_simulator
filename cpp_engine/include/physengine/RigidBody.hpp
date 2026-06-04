#ifndef PHYSENGINE_RIGIDBODY_HPP
#define PHYSENGINE_RIGIDBODY_HPP

#include "Vec3.hpp"
#include <vector>

struct Force {
    Vector3 force;
    Vector3 point;
};

class RigidBody {
public:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;

    double mass       = 1.0;
    bool   isStatic   = false;

    /* ── Boundary collision properties ──────────────────────────
     * restitution: energy kept after a boundary bounce
     *   1.0 = perfectly elastic (no energy lost)
     *   0.0 = body stops dead on contact
     *   0.75 is a good default (like a rubber ball)
     *
     * radius: half-width used as offset when clamping position
     *   so the surface of the ball touches the wall, not its centre
     * ──────────────────────────────────────────────────────────── */
    double restitution = 0.78;
    double radius      = 0.5;

    std::vector<Force> forces;

    RigidBody(double m = 1.0, Vector3 pos = Vector3(0, 0, 0));

    void applyForce(const Vector3& force, const Vector3& point = Vector3(0, 0, 0));
    void clearForces();

    /* Simple Euler update — used as fallback only.
     * RK4Integrator::integrate() is the preferred path. */
    void update(double dt);

    Vector3 getNetForce() const;
};

#endif /* PHYSENGINE_RIGIDBODY_HPP */
