#include "physengine/scenes/DoublePendulumScene.hpp"

void DoublePendulumScene::doublePendulumODE(double /*t*/, const double y[], double dydt[], void* params_ptr) {
    Params* p = static_cast<Params*>(params_ptr);

    double g = p->g, L1 = p->L1, L2 = p->L2, m1 = p->m1, m2 = p->m2;

    double theta1 = y[0], omega1 = y[1];
    double theta2 = y[2], omega2 = y[3];

    double delta = theta2 - theta1;

    // Denominators
    double den1 = (m1 + m2) * L1 - m2 * L1 * cos(delta) * cos(delta);
    double den2 = (L2 / L1) * den1;

    // Angular accelerations
    double alpha1 = (m2 * L1 * omega2 * omega2 * sin(delta) +
                     m2 * g * sin(theta2) * cos(delta) -
                     (m1 + m2) * g * sin(theta1)) / den1;

    double alpha2 = (-m2 * L2 * omega2 * omega2 * sin(delta) * cos(delta) +
                     (m1 + m2) * g * sin(theta1) * cos(delta) -
                     (m1 + m2) * g * sin(theta2)) / den2;

    dydt[0] = omega1;
    dydt[1] = alpha1;
    dydt[2] = omega2;
    dydt[3] = alpha2;
}

void DoublePendulumScene::init() {
    theta1 = M_PI * 0.8;   // ~144° — chaotic starting position
    omega1 = 0.0;
    theta2 = M_PI * 1.2;   // ~216°
    omega2 = 0.0;

    params_ = {gravity, length1, length2, mass1, mass2};

    if (state_) ode_state_free(state_);
    state_ = ode_state_create(4);
    state_->y[0] = theta1;
    state_->y[1] = omega1;
    state_->y[2] = theta2;
    state_->y[3] = omega2;
    state_->t = 0.0;
}

void DoublePendulumScene::update() {
    rk4_step(doublePendulumODE, state_, world.dt, &params_);

    theta1 = state_->y[0];
    omega1 = state_->y[1];
    theta2 = state_->y[2];
    omega2 = state_->y[3];
}

double DoublePendulumScene::getBob1X() const {
    return pivotX + length1 * sin(theta1);
}

double DoublePendulumScene::getBob1Y() const {
    return pivotY - length1 * cos(theta1);
}

double DoublePendulumScene::getBob2X() const {
    return getBob1X() + length2 * sin(theta2);
}

double DoublePendulumScene::getBob2Y() const {
    return getBob1Y() - length2 * cos(theta2);
}

DoublePendulumScene::~DoublePendulumScene() {
    if (state_) {
        ode_state_free(state_);
        state_ = nullptr;
    }
}