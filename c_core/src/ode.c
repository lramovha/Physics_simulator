#include "physmath/ode.h"
#include <stdlib.h>
#include <string.h>

ODEState* ode_state_create(int dim) {
    ODEState* s = (ODEState*)malloc(sizeof(ODEState));
    s->t = 0.0;
    s->dim = dim;
    s->y = (double*)calloc(dim, sizeof(double));
    return s;
}

void ode_state_free(ODEState* state) {
    if (state) {
        free(state->y);
        free(state);
    }
}

void ode_state_copy(ODEState* dest, const ODEState* src) {
    dest->t = src->t;
    dest->dim = src->dim;
    memcpy(dest->y, src->y, src->dim * sizeof(double));
}

int rk4_step(ODEFunc f, ODEState* state, double dt, void* params) {
    if (!state || dt <= 0.0) return -1;

    int n = state->dim;
    double* k1 = (double*)calloc(n, sizeof(double));
    double* k2 = (double*)calloc(n, sizeof(double));
    double* k3 = (double*)calloc(n, sizeof(double));
    double* k4 = (double*)calloc(n, sizeof(double));
    double* temp = (double*)calloc(n, sizeof(double));

    // k1 = f(t, y)
    f(state->t, state->y, k1, params);

    // k2 = f(t + dt/2, y + dt/2 * k1)
    for (int i = 0; i < n; i++) temp[i] = state->y[i] + 0.5 * dt * k1[i];
    f(state->t + 0.5 * dt, temp, k2, params);

    // k3 = f(t + dt/2, y + dt/2 * k2)
    for (int i = 0; i < n; i++) temp[i] = state->y[i] + 0.5 * dt * k2[i];
    f(state->t + 0.5 * dt, temp, k3, params);

    // k4 = f(t + dt, y + dt * k3)
    for (int i = 0; i < n; i++) temp[i] = state->y[i] + dt * k3[i];
    f(state->t + dt, temp, k4, params);

    // y_new = y + (dt/6)*(k1 + 2k2 + 2k3 + k4)
    for (int i = 0; i < n; i++) {
        state->y[i] += (dt / 6.0) * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
    }
    state->t += dt;

    free(k1); free(k2); free(k3); free(k4); free(temp);
    return 0;
}