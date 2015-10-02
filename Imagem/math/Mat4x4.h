#ifndef _MAT4X4_H_
#define _MAT4X4_H_

#include "Vec3.h"
#include "Point.h"

class Mat4x4 {
    public:
        Mat4x4();
        void loadIdentity();
        float& operator()(int, int);

        //TODO Funções para inverter a matriz e retornar
        //a transposta
        float mat[4][4];
};

Mat4x4 operator*(const Mat4x4& m1, const Mat4x4& m2);
Mat4x4 transpose(const Mat4x4& m);
#endif

