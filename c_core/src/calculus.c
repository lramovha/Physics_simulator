#include "physmath/calculus.h"
#include <math.h>

double derivative_forward(MathFunc f, double x, void* params, double h) {
    return (f(x + h, params) - f(x, params)) / h;
}

double derivative_central(MathFunc f, double x, void* params, double h) {
    return (f(x + h, params) - f(x - h, params)) / (2.0 * h);
}

double trapezoidal_rule(MathFunc f, double a, double b, int n, void* params) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a, params) + f(b, params));
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h, params);
    }
    return h * sum;
}

double simpson_rule(MathFunc f, double a, double b, int n, void* params) {
    if (n % 2 != 0) n++;  // Ensure even number of intervals
    
    double h = (b - a) / n;
    double sum = f(a, params) + f(b, params);
    
    for (int i = 1; i < n; i += 2) {
        sum += 4.0 * f(a + i * h, params);
    }
    for (int i = 2; i < n-1; i += 2) {
        sum += 2.0 * f(a + i * h, params);
    }
    return (h / 3.0) * sum;
}