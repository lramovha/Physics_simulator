#include "physmath/vec3.h"
#include <stdio.h>

Vec3 vec3_create(double x, double y, double z) {
    Vec3 v = {x, y, z};
    return v;
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return vec3_create(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_scale(Vec3 v, double scalar) {
    return vec3_create(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vec3 vec3_negate(Vec3 v) {
    return vec3_scale(v, -1.0);
}

double vec3_dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3_create(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

double vec3_magnitude(Vec3 v) {
    return sqrt(vec3_dot(v, v));
}

Vec3 vec3_normalize(Vec3 v) {
    double mag = vec3_magnitude(v);
    if (mag < 1e-12) return vec3_create(0,0,0);
    return vec3_scale(v, 1.0 / mag);
}

void vec3_print(Vec3 v) {
    printf("Vec3(%.4f, %.4f, %.4f)\n", v.x, v.y, v.z);
}