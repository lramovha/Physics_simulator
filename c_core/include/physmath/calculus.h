#ifndef PHYSMATH_CALCULUS_H
#define PHYSMATH_CALCULUS_H

#ifdef __cplusplus
extern "C" {
#endif

// Function pointer type
typedef double (*MathFunc)(double x, void* params);

// Numerical Differentiation
double derivative_forward(MathFunc f, double x, void* params, double h);
double derivative_central(MathFunc f, double x, void* params, double h);

// Numerical Integration
double trapezoidal_rule(MathFunc f, double a, double b, int n, void* params);
double simpson_rule(MathFunc f, double a, double b, int n, void* params);

#ifdef __cplusplus
}
#endif

#endif