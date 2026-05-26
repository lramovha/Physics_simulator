#ifndef PHYSMATH_ODE_H
#define PHYSMATH_ODE_H

#ifdef __cplusplus
extern "C" {
#endif

// State for ODE system
typedef struct {
    double t;
    double* y;      // state vector
    int dim;        // dimension of the system
} ODEState;

// Derivative function: dy/dt = f(t, y, params)
typedef void (*ODEFunc)(double t, const double y[], double dydt[], void* params);

// RK4 Step
int rk4_step(ODEFunc f, ODEState* state, double dt, void* params);

// Helper to create state
ODEState* ode_state_create(int dim);
void ode_state_free(ODEState* state);
void ode_state_copy(ODEState* dest, const ODEState* src);

#ifdef __cplusplus
}
#endif

#endif