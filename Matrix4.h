#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "Vec4.h"

class Matrix4
{
public:
    float values[4][4];

    Matrix4();
    Matrix4(float values[4][4]);
    Matrix4(const Matrix4 &other);
    Matrix4 operator=(const Matrix4 &other);

    Vec4 operator*(Vec4 &v) const;
    Matrix4 operator*(const Matrix4 &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m);
};

#endif