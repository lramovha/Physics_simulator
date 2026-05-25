#ifndef PHYSMATH_VEC3_H
#define PHYSMATH_VEC3_H

#include <math.h>

typedef struct {
    double x, y, z;
} Vec3;

#ifdef __cplusplus
extern "C" {
#endif

Vec3 vec3_create(double x, double y, double z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, double scalar);
Vec3 vec3_negate(Vec3 v);

double vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
double vec3_magnitude(Vec3 v);
Vec3 vec3_normalize(Vec3 v);

void vec3_print(Vec3 v);

#ifdef __cplusplus
}
#endif

#endif