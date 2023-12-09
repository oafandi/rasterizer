#ifndef __MATRIX4_H__
#define __MATRIX4_H__

class Matrix4
{
public:
    float values[4][4];

    Matrix4();
    Matrix4(float values[4][4]);
    Matrix4(const Matrix4 &other);
    
    Vec4 operator*(const Vec4 &v) const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m);
};

#endif