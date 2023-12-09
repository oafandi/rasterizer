#ifndef __VEC3_H__
#define __VEC3_H__
#define NO_COLOR -1

class Vec3
{
public:
    float x, y, z;
    int colorId;

    Vec3();
    Vec3(float x, float y, float z);
    Vec3(float x, float y, float z, int colorId);
    Vec3(const Vec3 &other);

    float getNthComponent(int n);
    float dot(const Vec3 &other);

    friend std::ostream &operator<<(std::ostream &os, const Vec3 &v);
};

#endif