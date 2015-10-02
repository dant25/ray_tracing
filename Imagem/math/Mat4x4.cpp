#include "Mat4x4.h"

Mat4x4::Mat4x4() {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(i == j)
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
}

void Mat4x4::loadIdentity() {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(i == j)
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
}

float& Mat4x4::operator()(int i, int j) {
    return mat[i][j];
}

Mat4x4 operator*(const Mat4x4& m1, const Mat4x4& m2) {
    Mat4x4 result;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) {
            result(i, j) = 0;
            for(int k = 0; k < 4; k++)
                result(i, j) += m1.mat[i][k]*m2.mat[k][j];
        }
    return result;
}

Mat4x4 transpose(const Mat4x4& m) {
    Mat4x4 result;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result(i, j) = m.mat[j][i];
}
