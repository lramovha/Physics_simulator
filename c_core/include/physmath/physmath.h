#ifndef PHYSMATH_H
#define PHYSMATH_H

// Version - Guarded to prevent redefinition warnings
#ifndef PHYSMATH_VERSION
#define PHYSMATH_VERSION "0.5.0"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void physmath_hello(void);

// Linear Algebra
#include "vec3.h"
#include "mat3.h"

// Calculus & ODE
#include "calculus.h"
#include "ode.h"

#ifdef __cplusplus
}
#endif

#endif