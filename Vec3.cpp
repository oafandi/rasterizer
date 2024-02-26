#include <iomanip>
#include "Vec3.h"
#include <cmath>

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = NO_COLOR;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = NO_COLOR;
}

Vec3::Vec3(float x, float y, float z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}

Vec3::Vec3(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}

float Vec3::getNthComponent(int n)
{
    switch (n)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
    default:
        return this->z;
    }
}

float Vec3::dot(const Vec3 &other)
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

void Vec3::normalize()
{
    float norm = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
}

Vec3 Vec3::cross(const Vec3 &other)
{
    float x = this->y * other.z - this->z * other.y;
    float y = this->z * other.x - this->x * other.z;
    float z = this->x * other.y - this->y * other.x;
    return Vec3(x, y, z);
}

std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    os << std::fixed << std::setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}