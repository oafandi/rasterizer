#ifndef FACE_H
#define FACE_H

#include "Vec4.h"
#include "Color.h"
#include "Vec3.h"

// Like Triangle but with vertex values instead of vertex ids

class Face {
public:
    Vec4 vertices[3];
    Color colors[3];
    Vec3 normal;
    bool isBackface = false;
    Face();
    Face(Vec4 &v1, Vec4 &v2, Vec4 &v3, Color c1, Color c2, Color c3);
    Face(const Face &other);
    
    void calculateNormal();
};

#endif //FACE_H