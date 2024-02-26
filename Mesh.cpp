#include <vector>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Mesh.h"
#include "Color.h"

Mesh::Mesh() {}

Mesh::Mesh(int meshId, int type, int numberOfTransformations,
           std::vector<int> transformationIds,
           std::vector<char> transformationTypes,
           int numberOfTriangles,
           std::vector<Triangle> triangles)
{
    this->meshId = meshId;
    this->type = type;
    this->numberOfTransformations = numberOfTransformations;
    this->numberOfTriangles = numberOfTriangles;
    this->transformationIds = transformationIds;
    this->transformationTypes = transformationTypes;
    this->triangles = triangles;

    float modelingValues [4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0 ,1, 0},
        {0, 0, 0, 1}
    };
    this->modelingTransformationMatrix = Matrix4(modelingValues);
}

void Mesh::setFaces(std::vector<Vec4> &vertices, std::vector<Color *> colorsOfVertices)
{
    for (int i = 0; i < this->triangles.size(); i++)
    {
        Face face = Face(vertices[this->triangles[i].vertexIds[0] - 1],
                         vertices[this->triangles[i].vertexIds[1] - 1],
                         vertices[this->triangles[i].vertexIds[2] - 1],
                         *colorsOfVertices[this->triangles[i].vertexIds[0]-1],
                         *colorsOfVertices[this->triangles[i].vertexIds[1]-1],
                         *colorsOfVertices[this->triangles[i].vertexIds[2]-1]);
        this->faces.push_back(face);
        // std::cout << "Faces before transformations: \n";
        // std::cout << "{" << face.vertices[0] << ", " << face.vertices[1] << ", " << face.vertices[2] << "}" << std::endl;
    }
}

void Mesh::setVertices(std::vector<Vec4> &vertices)
{  /*
    for (int i = 0; i < triangles.size(); i++)
    {
        if (find(vertices.begin(), vertices.end(), vertices[triangles[i].vertexIds[0] - 1]) == this->vertices.end()) this->vertices.push_back(vertices[triangles[i].vertexIds[0] - 1]);
        if (find(vertices.begin(), vertices.end(), vertices[triangles[i].vertexIds[1] - 1]) == this->vertices.end()) this->vertices.push_back(vertices[triangles[i].vertexIds[1] - 1]);
        if (find(vertices.begin(), vertices.end(), vertices[triangles[i].vertexIds[2] - 1]) == this->vertices.end()) this->vertices.push_back(vertices[triangles[i].vertexIds[2] - 1]);
    }
    */
}

std::ostream &operator<<(std::ostream &os, const Mesh &m)
{
    os << "Mesh " << m.meshId;

    if (m.type == 0)
    {
        os << " wireframe(0) with ";
    }
    else
    {
        os << " solid(1) with ";
    }

    os << std::fixed << std::setprecision(3) << m.numberOfTransformations << " transformations and " << m.numberOfTriangles << " triangles"
       << std::endl
       << "\tTriangles are:" << std::endl
       << std::fixed << std::setprecision(0);

    for (int i = 0; i < m.triangles.size(); i++)
    {
        os << "\t\t" << m.triangles[i].vertexIds[0] << " " << m.triangles[i].vertexIds[1] << " " << m.triangles[i].vertexIds[2] << std::endl;
    }

    return os;
}