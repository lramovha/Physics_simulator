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
    
    double mass;
    bool isStatic;

    std::vector<Force> forces;

    RigidBody(double m = 1.0, Vector3 pos = Vector3(0,0,0));

    void applyForce(const Vector3& force, const Vector3& point = Vector3(0,0,0));
    void clearForces();
    
    void update(double dt);

    Vector3 getNetForce() const;
};

#endif