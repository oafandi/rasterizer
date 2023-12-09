#ifndef __VEC4_H__
#define __VEC4_H__
#define NO_COLOR -1

class Vec4
{
public:
    float x, y, z, t;
    int colorId;

    Vec4();
    Vec4(float x, float y, float z, float t);
    Vec4(float x, float y, float z, float t, int colorId);
    Vec4(const Vec4 &other);
    Vec4 operator/(float divisor) const;

    float getNthComponent(int n);

    friend std::ostream &operator<<(std::ostream &os, const Vec4 &v);
};

#endif