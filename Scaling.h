#ifndef __SCALING_H__
#define __SCALING_H__

#include "Matrix4.h"

class Scaling
{
public:
    int scalingId;
    Matrix4 matrix;

    Scaling();
    Scaling(int scalingId, double sx, double sy, double sz);
    friend std::ostream &operator<<(std::ostream &os, const Scaling &s);
};

#endif