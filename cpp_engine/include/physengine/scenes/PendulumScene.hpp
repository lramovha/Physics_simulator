#ifndef PENDULUM_SCENE_HPP
#define PENDULUM_SCENE_HPP

#include "BaseScene.hpp"
#include "physmath/ode.h"

/* ─────────────────────────────────────────────────────────────────
 * PendulumScene
 *
 * Encodes the simple pendulum ODE directly:
 *   θ' = ω
 *   ω' = -(g/L) sin θ
 *
 * Does NOT use RigidBody — the pendulum is a constrained system,
 * not a free particle. It manages its own ODEState and calls
 * rk4_step() from the C math core on every update().
 *
 * The pivot is fixed. The bob position is derived from (theta, L).
 * ───────────────────────────────────────────────────────────────── */
class PendulumScene : public BaseScene {
public:
    /* Pivot position in world coordinates */
    double pivotX = 0.0;
    double pivotY = 8.0;

    /* Pendulum parameters */
    double length = 5.0;   /* rod length in metres */
    double gravity = 9.81;

    /* State — read by JNI and renderer */
    double theta = 0.0;    /* angle from vertical in radians */
    double omega = 0.0;    /* angular velocity in rad/s     */

    /* BaseScene interface */
    void init()   override;
    void update() override;   /* does NOT call world.step() */
    const char* getName() const override { return "Simple Pendulum"; }

    /* World-space position of the bob (derived from theta) */
    double getBobX() const;
    double getBobY() const;

    ~PendulumScene() override;

private:
    ODEState* state_ = nullptr;

    /* Parameters struct passed into the ODE function */
    struct Params { double g; double L; };
    Params params_;

    /* ODE function:  dydt = f(t, y, params)
     *   y[0] = theta,  y[1] = omega            */
    static void pendulumODE(double t,
                            const double y[],
                            double dydt[],
                            void*  params);
};

#endif /* PENDULUM_SCENE_HPP */


// #ifndef PENDULUM_SCENE_HPP
// #define PENDULUM_SCENE_HPP

// #include "BaseScene.hpp"

// class PendulumScene : public BaseScene {
// public:
//     void init() override;
//     const char* getName() const override { return "Simple Pendulum"; }
// };

// #endif