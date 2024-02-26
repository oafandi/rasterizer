#include "Face.h"
#include "Vec3.h"

Face::Face()
{
    this->vertices[0] = Vec4();
    this->vertices[1] = Vec4();
    this->vertices[2] = Vec4();
    this->normal = Vec3();
    this->colors[0] = Color(255, 0, 0);
    this->colors[1] = Color(255, 0, 0);
    this->colors[2] = Color(255, 0, 0);
    this->isBackface = false;
}

Face::Face(Vec4 &v1, Vec4 &v2, Vec4 &v3, Color c1, Color c2, Color c3)
{
    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;
    this->normal = Vec3();
    this->colors[0] = c1;
    this->colors[1] = c2;
    this->colors[2] = c3;
    this->isBackface = false;
}

Face::Face(const Face &other)
{
    this->vertices[0] = other.vertices[0];
    this->vertices[1] = other.vertices[1];
    this->vertices[2] = other.vertices[2];
    this->normal = other.normal;
    this->colors[0] = other.colors[0];
    this->colors[1] = other.colors[1];
    this->colors[2] = other.colors[2];
    this->isBackface = other.isBackface;
}

void Face::calculateNormal()
{
    Vec3 v1 = Vec3(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z);
    Vec3 v2 = Vec3(vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y, vertices[2].z - vertices[0].z);
    normal = v1.cross(v2);
    normal.normalize();
}