#ifndef DOUBLE_PENDULUM_SCENE_HPP
#define DOUBLE_PENDULUM_SCENE_HPP

#include "BaseScene.hpp"
#include "physmath/ode.h"
#include <cmath>

class DoublePendulumScene : public BaseScene {
public:
    /* Pivot position */
    double pivotX = 0.0;
    double pivotY = 10.0;

    /* Parameters */
    double length1 = 4.0;   // upper arm
    double length2 = 3.5;   // lower arm
    double mass1   = 1.0;
    double mass2   = 1.0;
    double gravity = 9.81;

    /* State — read by JNI */
    double theta1 = 0.0;   // upper angle
    double omega1 = 0.0;   // upper angular vel
    double theta2 = 0.0;   // lower angle
    double omega2 = 0.0;   // lower angular vel

    void init() override;
    void update() override;
    const char* getName() const override { return "Double Pendulum"; }

    /* Get positions for rendering */
    double getBob1X() const;
    double getBob1Y() const;
    double getBob2X() const;
    double getBob2Y() const;

    ~DoublePendulumScene() override;

private:
    ODEState* state_ = nullptr;

    struct Params {
        double g, L1, L2, m1, m2;
    };
    Params params_;

    static void doublePendulumODE(double t, const double y[], double dydt[], void* params);
};

#endif