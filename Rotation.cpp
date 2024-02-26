#include <iomanip>
#include "Rotation.h"
#include "Vec3.h"
#include "Matrix4.h"
#include <iostream>
#include <cmath>

const float PI = 3.14159265358979323846;

Rotation::Rotation() {
    this->rotationId = -1;
}
/*
Rotation::Rotation(int rotationId, float angle, float x, float y, float z)
{
    this->rotationId = rotationId;
    Vec3 u(x, y, z);
    Vec3 v;
    u.normalize();
    float xabs = 0, yabs = 0, zabs = 0;
    
    if (u.x < 0) xabs = -u.x;
    else xabs = u.x;
    if (u.y < 0) yabs = -u.y;
    else yabs = y;
    if (u.z < 0) zabs = -u.z;
    else zabs = u.z;

    if (xabs < yabs and xabs < zabs) {
        v.x = 0;
        v.y = -u.z;
        v.z = u.y;
    }
    else if (yabs < xabs and yabs < zabs) {
        v.x = -u.z;
        v.y = 0;
        v.z = u.x;
    }
    else {
        v.x = -u.y;
        v.y = u.x;
        v.z = 0;
    }

    Vec3 w = u.cross(v);
    v.normalize();
    w.normalize();

    // std::cout << "u: " << u << std::endl;
    // std::cout << "v: " << v << std::endl;

    float axesAlignment[4][4] = {
        {u.x, u.y, u.z, 0},
        {v.x, v.y, v.z, 0},
        {w.x, w.y, w.z, 0},
        {0, 0, 0, 1}
    };
    float axesAlignmentInverse[4][4] = {
        {u.x, v.x, w.x, 0},
        {u.y, v.y, w.y, 0},
        {u.z, v.z, w.z, 0},
        {0, 0, 0, 1}
    };
    float rotationAroundX[4][4] = {
        {1, 0, 0, 0},
        {0, cosf(PI * angle / 180.0f), -sinf(PI * angle / 180.0f), 0},
        {0, sinf(PI * angle / 180.0f), cosf(PI * angle / 180.0f), 0},
        {0, 0, 0, 1}
    };
    Matrix4 axesAlignmentMatrix(axesAlignment);
    Matrix4 axesAlignmentMatrixInverse(axesAlignmentInverse);
    Matrix4 rotationAroundXMatrix(rotationAroundX);

    // std::cout << "axesAlignmentMatrix: " << axesAlignmentMatrix << std::endl;
    // std::cout << "axesAlignmentMatrixInverse: " << axesAlignmentMatrixInverse << std::endl;
    // std::cout << "rotationAroundXMatrix: " << rotationAroundXMatrix << std::endl;

    this->matrix = axesAlignmentMatrix;
    this->matrix = rotationAroundXMatrix * this->matrix;
    this->matrix = axesAlignmentMatrixInverse * this->matrix;

    std::cout << "this->matrix: " << this->matrix << std::endl;
} */

Rotation::Rotation(int rotationId, float angle, float x, float y, float z) {
    Vec3 u(x, y, z);
    u.normalize();
    if (y == 0 and z == 0) {
        float rotationAroundX[4][4] = {
            {1, 0, 0, 0},
            {0, cosf(PI * angle / 180.0f), -sinf(PI * angle / 180.0f), 0},
            {0, sinf(PI * angle / 180.0f), cosf(PI * angle / 180.0f), 0},
            {0, 0, 0, 1}
        };
        this->matrix = Matrix4(rotationAroundX);
    }
    else if (x == 0 and z == 0) {
        float rotationAroundY[4][4] = {
            {cosf(PI * angle / 180.0f), 0, sinf(PI * angle / 180.0f), 0},
            {0, 1, 0, 0},
            {-sinf(PI * angle / 180.0f), 0, cosf(PI * angle / 180.0f), 0},
            {0, 0, 0, 1}
        };
        this->matrix = Matrix4(rotationAroundY);
    }
    else if (x == 0 and y == 0) {
        float rotationAroundZ[4][4] = {
            {cosf(PI * angle / 180.0f), -sinf(PI * angle / 180.0f), 0, 0},
            {sinf(PI * angle / 180.0f), cosf(PI * angle / 180.0f), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        this->matrix = Matrix4(rotationAroundZ);
    }
    else {
        float d = sqrtf(u.y * u.y + u.z * u.z);
        float rotationAroundX[4][4] = {
            {1, 0, 0, 0},
            {0, u.z/d, -u.y/d, 0},
            {0, u.y/d, u.z/d, 0},
            {0, 0, 0, 1}
        };
        float rotationAroundY[4][4] = {
            {d, 0, -u.x, 0},
            {0, 1, 0, 0},
            {u.x, 0, d, 0},
            {0, 0, 0, 1}
        };
        float rotationAroundZ[4][4] = {
            {cosf(PI * angle / 180.0f), -sinf(PI * angle / 180.0f), 0, 0},
            {sinf(PI * angle / 180.0f), cosf(PI * angle / 180.0f), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        float rotationAroundYInverse[4][4] = {
            {d, 0, u.x, 0},
            {0, 1, 0, 0},
            {-u.x, 0, d, 0},
            {0, 0, 0, 1}
        };
        float rotationAroundXInverse[4][4] = {
            {1, 0, 0, 0},
            {0, u.z/d, u.y/d, 0},
            {0, -u.y/d, u.z/d, 0},
            {0, 0, 0, 1}
        };

        Matrix4 rotationAroundXMatrix(rotationAroundX);
        Matrix4 rotationAroundYMatrix(rotationAroundY);
        Matrix4 rotationAroundZMatrix(rotationAroundZ);
        Matrix4 rotationAroundYInverseMatrix(rotationAroundYInverse);
        Matrix4 rotationAroundXInverseMatrix(rotationAroundXInverse);

        this->matrix = rotationAroundXMatrix;
        this->matrix = rotationAroundYMatrix * this->matrix;
        this->matrix = rotationAroundZMatrix * this->matrix;
        this->matrix = rotationAroundYInverseMatrix * this->matrix;
        this->matrix = rotationAroundXInverseMatrix * this->matrix;
    }
}

/*
TEST:
Axis: (0.8, 0.6, 0.0)
Angle: 60
Rotation Matrix = [  0.8200000,  0.2400000,  0.5196152;
                     0.2400000,  0.6800000, -0.6928203;
                    -0.5196152,  0.6928203,  0.5000000 ]

Rotation Around X Matrix = 
*/

/*
std::ostream &operator<<(std::ostream &os, const Rotation &r)
{
    os << std::fixed << std::setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.ux << ", " << r.uy << ", " << r.uz << "]";
    return os;
}
*/