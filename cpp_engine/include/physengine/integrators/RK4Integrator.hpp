#ifndef RK4_INTEGRATOR_HPP
#define RK4_INTEGRATOR_HPP

#include "../RigidBody.hpp"

class RK4Integrator {
public:
    static void integrate(RigidBody& body, double dt);
};

#endif