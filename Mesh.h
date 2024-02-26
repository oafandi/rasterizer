#ifndef __MESH_H__
#define __MESH_H__
#define WIREFRAME_MESH 0
#define SOLID_MESH 1
#include "Triangle.h"
#include "Matrix4.h"
#include "Face.h"
#include <vector>

class Mesh
{

public:
    int meshId, type, numberOfTransformations, numberOfTriangles; // type=0 for wireframe, type=1 for solid
    std::vector<int> transformationIds;
    std::vector<char> transformationTypes;
    std::vector<Triangle> triangles;
    std::vector<Face> faces; // deep copied from triangles
    std::vector<Vec4> vertices;
    Matrix4 modelingTransformationMatrix;
    
    Mesh();
    Mesh(int meshId, int type, int numberOfTransformations,
         std::vector<int> transformationIds,
         std::vector<char> transformationTypes,
         int numberOfTriangles,
         std::vector<Triangle> triangles);

    void setFaces(std::vector<Vec4> &vertices, std::vector<Color *> colorsOfVertices);
    void setVertices(std::vector<Vec4> &vertices);
    
    friend std::ostream &operator<<(std::ostream &os, const Mesh &m);
};

#endif