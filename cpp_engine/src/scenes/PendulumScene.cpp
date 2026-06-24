#include "physengine/scenes/PendulumScene.hpp"
#include <cmath>

/* ─────────────────────────────────────────────────────────────────
 * ODE function
 *   y[0] = θ  (angle from vertical, radians)
 *   y[1] = ω  (angular velocity, rad/s)
 *
 *   dθ/dt = ω
 *   dω/dt = -(g/L) sinθ
 *
 * This is the FULL nonlinear pendulum — no small-angle approximation.
 * For small angles (< ~10°) it reduces to SHM with period 2π√(L/g).
 * ───────────────────────────────────────────────────────────────── */
void PendulumScene::pendulumODE(double /*t*/,
                                const double y[],
                                double dydt[],
                                void*  params)
{
    Params* p = static_cast<Params*>(params);
    dydt[0] = y[1];                                      /* dθ/dt = ω        */
    dydt[1] = -(p->g / p->L) * std::sin(y[0]);          /* dω/dt = -(g/L)sinθ */
}

void PendulumScene::init() {
    /* Initial conditions: 45-degree release from rest */
    theta   = M_PI / 4.0;   /* 45° */
    omega   = 0.0;
    length  = 5.0;
    gravity = 9.81;

    params_ = { gravity, length };

    /* Allocate (or re-allocate) ODE state — dim=2: [theta, omega] */
    if (state_) {
        ode_state_free(state_);
    }
    state_       = ode_state_create(2);
    state_->y[0] = theta;
    state_->y[1] = omega;
    state_->t    = 0.0;
}

void PendulumScene::update() {
    /* Advance the ODE by one timestep using the C math core RK4 solver.
     * Note: we do NOT call world.step() here — the pendulum has no
     * RigidBody, so the PhysicsWorld is unused for physics in this scene.
     * world.dt is reused as the timestep so the scene respects any
     * global timestep setting. */
    rk4_step(pendulumODE, state_, world.dt, &params_);

    /* Sync readable fields with the ODE state */
    theta = state_->y[0];
    omega = state_->y[1];
}

double PendulumScene::getBobX() const {
    /*  x_bob = x_pivot + L * sin(θ)  */
    return pivotX + length * std::sin(theta);
}

double PendulumScene::getBobY() const {
    /*  y_bob = y_pivot - L * cos(θ)
     *  (minus because θ=0 means bob hangs straight DOWN from pivot) */
    return pivotY - length * std::cos(theta);
}

PendulumScene::~PendulumScene() {
    if (state_) {
        ode_state_free(state_);
        state_ = nullptr;
    }
}
