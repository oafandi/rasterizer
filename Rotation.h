#ifndef __ROTATION_H__
#define __ROTATION_H__

#include "Matrix4.h"

class Rotation
{
public:
    int rotationId;
    Matrix4 matrix;

    Rotation();
    Rotation(int rotationId, float angle, float x, float y, float z);
    friend std::ostream &operator<<(std::ostream &os, const Rotation &r);
};

#endif