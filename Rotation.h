#ifndef __ROTATION_H__
#define __ROTATION_H__

class Rotation
{
public:
    int rotationId;
    Matrix4 matrix;

    Rotation();
    Rotation(int rotationId, double angle, double x, double y, double z);
    friend std::ostream &operator<<(std::ostream &os, const Rotation &r);
};

#endif