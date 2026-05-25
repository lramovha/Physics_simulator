#include "physmath/mat3.h"
#include <stdio.h>

Mat3 mat3_identity(void) {
    Mat3 m = {{{1,0,0},{0,1,0},{0,0,1}}};
    return m;
}

Mat3 mat3_scale(double s) {
    Mat3 m = {{{s,0,0},{0,s,0},{0,0,s}}};
    return m;
}

Mat3 mat3_from_rows(Vec3 r1, Vec3 r2, Vec3 r3) {
    Mat3 m = {{{r1.x, r1.y, r1.z},
               {r2.x, r2.y, r2.z},
               {r3.x, r3.y, r3.z}}};
    return m;
}

Mat3 mat3_mult(Mat3 a, Mat3 b) {
    Mat3 c = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                c.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return c;
}

Vec3 mat3_mult_vec3(Mat3 m, Vec3 v) {
    return vec3_create(
        m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z,
        m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z,
        m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z
    );
}

void mat3_print(Mat3 m) {
    printf("Mat3:\n");
    for (int i = 0; i < 3; i++) {
        printf("  [ %.4f  %.4f  %.4f ]\n", m.m[i][0], m.m[i][1], m.m[i][2]);
    }
}