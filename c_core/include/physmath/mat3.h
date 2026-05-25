#ifndef PHYSMATH_MAT3_H
#define PHYSMATH_MAT3_H

#include "vec3.h"

typedef struct {
    double m[3][3];
} Mat3;

#ifdef __cplusplus
extern "C" {
#endif

Mat3 mat3_identity(void);
Mat3 mat3_scale(double s);
Mat3 mat3_from_rows(Vec3 r1, Vec3 r2, Vec3 r3);

Mat3 mat3_mult(Mat3 a, Mat3 b);
Vec3 mat3_mult_vec3(Mat3 m, Vec3 v);

void mat3_print(Mat3 m);

#ifdef __cplusplus
}
#endif

#endif
