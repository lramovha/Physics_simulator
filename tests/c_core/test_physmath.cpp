// #include <catch2/catch_test_macros.hpp>
// #include "physmath/physmath.h"

// TEST_CASE("Physmath hello works") {
//     REQUIRE_NOTHROW(physmath_hello());
// }

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "physmath/physmath.h"
#include <iostream>

using Catch::Matchers::WithinAbs;

TEST_CASE("PhysMath - Version", "[physmath]") {
    REQUIRE(std::string(PHYSMATH_VERSION).length() > 0);
}

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