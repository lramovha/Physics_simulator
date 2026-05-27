#include "physengine/integrators/RK4Integrator.hpp"
#include "physmath/ode.h"
#include <cstdlib>

namespace {
    void rigidbody_ode_func(double t, const double y[], double dydt[], void* params) {
        RigidBody* body = static_cast<RigidBody*>(params);
        Vector3 netForce = body->getNetForce();
        Vector3 acc = netForce * (1.0 / body->mass);

        dydt[0] = body->velocity.x();
        dydt[1] = body->velocity.y();
        dydt[2] = body->velocity.z();
        dydt[3] = acc.x();
        dydt[4] = acc.y();
        dydt[5] = acc.z();
    }
}

void RK4Integrator::integrate(RigidBody& body, double dt) {
    if (body.isStatic || body.mass <= 0.0) return;

    ODEState* state = ode_state_create(6);
    state->t = 0.0;

    state->y[0] = body.position.x();
    state->y[1] = body.position.y();
    state->y[2] = body.position.z();
    state->y[3] = body.velocity.x();
    state->y[4] = body.velocity.y();
    state->y[5] = body.velocity.z();

    rk4_step(rigidbody_ode_func, state, dt, &body);

    body.position = Vector3(state->y[0], state->y[1], state->y[2]);
    body.velocity = Vector3(state->y[3], state->y[4], state->y[5]);

    ode_state_free(state);
}