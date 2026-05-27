#ifndef PHYSENGINE_VECTOR3_HPP
#define PHYSENGINE_VECTOR3_HPP

#include "physmath/vec3.h"
#include <iostream>

struct Vector3 {
    ::Vec3 v;   // Holds the C struct

    Vector3() { v = vec3_create(0, 0, 0); }
    Vector3(double x, double y, double z) { v = vec3_create(x, y, z); }
    Vector3(::Vec3 cv) : v(cv) {}

    double x() const { return v.x; }
    double y() const { return v.y; }
    double z() const { return v.z; }

    // Operators
    Vector3 operator+(const Vector3& other) const { return Vector3(vec3_add(v, other.v)); }
    Vector3 operator-(const Vector3& other) const { return Vector3(vec3_sub(v, other.v)); }
    Vector3 operator*(double scalar) const { return Vector3(vec3_scale(v, scalar)); }
    Vector3 operator-() const { return Vector3(vec3_negate(v)); }

    double dot(const Vector3& other) const { return vec3_dot(v, other.v); }
    Vector3 cross(const Vector3& other) const { return Vector3(vec3_cross(v, other.v)); }
    double magnitude() const { return vec3_magnitude(v); }
    Vector3 normalized() const { return Vector3(vec3_normalize(v)); }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
        vec3_print(vec.v);
        return os;
    }
};

#endif