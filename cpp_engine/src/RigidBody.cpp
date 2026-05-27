#include "physengine/RigidBody.hpp"

RigidBody::RigidBody(double m, Vector3 pos) 
    : mass(m), 
      position(pos), 
      velocity(Vector3(0,0,0)), 
      acceleration(Vector3(0,0,0)), 
      isStatic(false) {}

void RigidBody::applyForce(const Vector3& force, const Vector3& point) {
    forces.push_back({force, point});
}

void RigidBody::clearForces() {
    forces.clear();
}

Vector3 RigidBody::getNetForce() const {
    Vector3 net(0,0,0);
    for (const auto& f : forces) {
        net = net + f.force;
    }
    return net;
}

void RigidBody::update(double dt) {
    if (isStatic || mass <= 0.0) return;

    Vector3 netForce = getNetForce();
    acceleration = netForce * (1.0 / mass);

    velocity = velocity + acceleration * dt;
    position = position + velocity * dt;

    clearForces();
}