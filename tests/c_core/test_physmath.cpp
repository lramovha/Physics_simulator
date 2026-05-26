// #include <catch2/catch_test_macros.hpp>
// #include "physmath/physmath.h"

// TEST_CASE("Physmath hello works") {
//     REQUIRE_NOTHROW(physmath_hello());
// }

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "physmath/physmath.h"
#include <iostream>
#include <cmath>

using Catch::Matchers::WithinAbs;

TEST_CASE("PhysMath - Version", "[physmath]") {
    REQUIRE(std::string(PHYSMATH_VERSION).length() > 0);
}

// ==================== PHASE 2: LINEAR ALGEBRA ====================
TEST_CASE("Vec3 - Creation and Basic Operations", "[vec3]") {
    Vec3 a = vec3_create(1.0, 2.0, 3.0);
    Vec3 b = vec3_create(4.0, 5.0, 6.0);

    SECTION("Creation") {
        REQUIRE(a.x == 1.0);
        REQUIRE(a.y == 2.0);
        REQUIRE(a.z == 3.0);
    }

    SECTION("Addition and Subtraction") {
        Vec3 sum = vec3_add(a, b);
        REQUIRE(sum.x == 5.0);
        REQUIRE(sum.y == 7.0);
        REQUIRE(sum.z == 9.0);

        Vec3 diff = vec3_sub(b, a);
        REQUIRE(diff.x == 3.0);
        REQUIRE(diff.y == 3.0);
        REQUIRE(diff.z == 3.0);
    }

    SECTION("Scaling and Negation") {
        Vec3 scaled = vec3_scale(a, 2.0);
        REQUIRE(scaled.x == 2.0);
        REQUIRE(scaled.y == 4.0);
        REQUIRE(scaled.z == 6.0);

        Vec3 negated = vec3_negate(a);
        REQUIRE(negated.x == -1.0);
        REQUIRE(negated.y == -2.0);
        REQUIRE(negated.z == -3.0);
    }
}

TEST_CASE("Vec3 - Vector Math", "[vec3]") {
    Vec3 a = vec3_create(3.0, 4.0, 0.0);
    Vec3 b = vec3_create(0.0, 0.0, 5.0);

    SECTION("Dot Product") {
        double dot = vec3_dot(a, b);
        REQUIRE(dot == 0.0);  // perpendicular
    }

    SECTION("Cross Product") {
        Vec3 cross = vec3_cross(a, b);
        REQUIRE(cross.x == 20.0);
        REQUIRE(cross.y == -15.0);
        REQUIRE(cross.z == 0.0);
    }

    SECTION("Magnitude and Normalize") {
        double mag = vec3_magnitude(a);
        REQUIRE_THAT(mag, WithinAbs(5.0, 1e-9));

        Vec3 unit = vec3_normalize(a);
        REQUIRE_THAT(vec3_magnitude(unit), WithinAbs(1.0, 1e-9));
    }
}

TEST_CASE("Mat3 - Matrix Operations", "[mat3]") {
    Mat3 I = mat3_identity();
    Mat3 S = mat3_scale(2.0);

    SECTION("Identity Matrix") {
        REQUIRE(I.m[0][0] == 1.0);
        REQUIRE(I.m[1][1] == 1.0);
        REQUIRE(I.m[2][2] == 1.0);
    }

    SECTION("Matrix Multiplication") {
        Mat3 result = mat3_mult(I, S);
        REQUIRE(result.m[0][0] == 2.0);
        REQUIRE(result.m[1][1] == 2.0);
    }

    SECTION("Matrix-Vector Multiplication") {
        Vec3 v = vec3_create(1.0, 2.0, 3.0);
        Vec3 result = mat3_mult_vec3(S, v);
        REQUIRE(result.x == 2.0);
        REQUIRE(result.y == 4.0);
        REQUIRE(result.z == 6.0);
    }
}

TEST_CASE("PhysMath - Integration", "[physmath]") {
    Vec3 v = vec3_create(1, 2, 3);
    Mat3 m = mat3_identity();
    Vec3 result = mat3_mult_vec3(m, v);
    
    REQUIRE(result.x == 1.0);
    REQUIRE(result.y == 2.0);
    REQUIRE(result.z == 3.0);
}

// ==================== PHASE 3: CALCULUS & ODE ====================

TEST_CASE("Calculus - Numerical Differentiation", "[calculus]") {
    auto f = [](double x, void* params) -> double {
        return x * x;  // f(x) = x²
    };

    SECTION("Forward Difference") {
        double df = derivative_forward(f, 3.0, NULL, 1e-5);
        REQUIRE_THAT(df, WithinAbs(6.0, 1e-3));
    }

    SECTION("Central Difference") {
        double df = derivative_central(f, 3.0, NULL, 1e-5);
        REQUIRE_THAT(df, WithinAbs(6.0, 1e-6));
    }
}

TEST_CASE("Calculus - Numerical Integration", "[calculus]") {
    auto f = [](double x, void* params) -> double {
        return sin(x);  // ∫sin(x) dx from 0 to π = 2
    };

    SECTION("Trapezoidal Rule") {
        double result = trapezoidal_rule(f, 0.0, M_PI, 1000, NULL);
        REQUIRE_THAT(result, WithinAbs(2.0, 1e-3));
    }

    SECTION("Simpson's Rule") {
        double result = simpson_rule(f, 0.0, M_PI, 1000, NULL);
        REQUIRE_THAT(result, WithinAbs(2.0, 1e-5));
    }
}

TEST_CASE("ODE - RK4 Simple Integration", "[ode]") {
    // Test: dy/dt = -y , y(0)=1  → analytical y = e^(-t)
    auto simple_decay = [](double t, const double y[], double dydt[], void* params) {
        dydt[0] = -y[0];
    };

    ODEState* state = ode_state_create(1);
    state->y[0] = 1.0;

    const double dt = 0.1;
    const int steps = 20;  // t = 2.0

    for (int i = 0; i < steps; i++) {
        rk4_step(simple_decay, state, dt, NULL);
    }

    double expected = exp(-2.0);
    REQUIRE_THAT(state->y[0], WithinAbs(expected, 1e-4));

    ode_state_free(state);
}

TEST_CASE("ODE - State Management", "[ode]") {
    ODEState* s1 = ode_state_create(2);
    s1->y[0] = 10.0;
    s1->y[1] = 20.0;

    ODEState* s2 = ode_state_create(2);
    ode_state_copy(s2, s1);

    REQUIRE(s2->y[0] == 10.0);
    REQUIRE(s2->y[1] == 20.0);

    ode_state_free(s1);
    ode_state_free(s2);
}