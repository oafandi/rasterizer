#ifndef __CAMERA_H__
#define __CAMERA_H__
#define ORTOGRAPHIC_PROJECTION 0
#define PERSPECTIVE_PROJECTION 1
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix4.h"
#include "Triangle.h"
#include "Line.h"
#include "Face.h"
#include "Mesh.h"
#include "Color.h"
#include <vector>

class Camera
{
public:
    int cameraId, projectionType, horRes, verRes; // projectionType=0 for orthographic, projectionType=1 for perspective
    Vec3 position, gaze, u, v, w;
    double left, right, bottom, top, near, far;
    std::string outputFilename;
    std::vector<Vec4> vertices;
    std::vector<Line> lines;
    std::vector<Face> triangles;
    std::vector<Mesh> meshes;
    Matrix4 cameraTransformationMatrix;
    Matrix4 projectionTransformationMatrix;
    Matrix4 viewportTransformationMatrix;

	std::vector<std::vector<Color> > image; 
	std::vector<std::vector<double> > depth;

	Color backgroundColor; // TODO init
	bool cullingEnabled; // TODO init

    Camera();

    Camera(int cameraId,
           int projectionType,
           Vec3 position, Vec3 gaze,
           Vec3 u, Vec3 v, Vec3 w,
           double left, double right, double bottom, double top,
           double near, double far,
           int horRes, int verRes,
           Color backgroundColor, bool cullingEnabled,
           std::string outputFilename);

    Camera(const Camera &other);
    
    // void setVertices(std::vector<Vec4> &vertices);
    void setMeshes(std::vector<Mesh*> meshes);
    void setCameraTransformationMatrix();
    void setProjectionTransformationMatrix();
    void setViewportTransformationMatrix();

    Vec4 getVertex(int id);

    void applyCameraTransformation();
    void applyProjectionTransformation();
    void applyViewportTransformation();

    void render();

    void primitiveAssembly(bool isCullingEnabled);
    bool clipping(int& x1, int& y1, int& x2, int& y2, float& z1, float& z2, Color& c1, Color& c2, int xmin, int xmax, int ymin, int ymax);

    void backfaceCulling();

    // drawing + color
    void drawPixel(int x, int y, float z, Color c);
    void drawLine(int x1, int y1, int x2, int y2, float z1, float z2, Color c1, Color c2);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, 
                        float z1, float z2, float z3,
                        Color c1, Color c2, Color c3);
    bool isInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
    Color calculateLineColor(Color c1, Color c2, float t);
    Color calculateTriangleColor(Color c1, Color c2, Color c3, float alpha, float beta, float gamma);

    bool isWithinImage(int x, int y);
    int calculateLineEquation(int x1, int y1, int x2, int y2, int x, int y);

	// camera related static functions
	void assignColorToPixel(int i, int j, Color c);
	void initializeImage();
	int  makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile();
	void convertPPMToPNG(std::string ppmFileName);

    friend std::ostream &operator<<(std::ostream &os, const Camera &c);
};

#endif