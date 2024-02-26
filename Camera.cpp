#include <iomanip>
#include <iostream>
#include "Camera.h"
#include "Vec4.h"
#include "Mesh.h"
#include "Line.h"
#include "Face.h"
#include "Color.h"
#include <vector>
#include <ostream>
#include <fstream>
#include <algorithm>

using namespace std;

Camera::Camera() {}
Camera::Camera(int cameraId,
               int projectionType,
               Vec3 position, Vec3 gaze,
               Vec3 u, Vec3 v, Vec3 w,
               double left, double right, double bottom, double top,
               double near, double far,
               int horRes, int verRes,
               Color backgroundColor, bool cullingEnabled,
               std::string outputFilename)
{

    this->cameraId = cameraId;
    this->projectionType = projectionType;
    this->position = position;
    this->gaze = gaze;
    this->u = u;
    this->v = v;
    this->w = w;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->near = near;
    this->far = far;
    this->horRes = horRes;
    this->verRes = verRes;
    this->outputFilename = outputFilename;
    this->backgroundColor = backgroundColor;
    this->cullingEnabled = cullingEnabled;
}
Camera::Camera(const Camera &other)
{
    this->cameraId = other.cameraId;
    this->projectionType = other.projectionType;
    this->position = other.position;
    this->gaze = other.gaze;
    this->u = other.u;
    this->v = other.v;
    this->w = other.w;
    this->left = other.left;
    this->right = other.right;
    this->bottom = other.bottom;
    this->top = other.top;
    this->near = other.near;
    this->far = other.far;
    this->horRes = other.horRes;
    this->verRes = other.verRes;
    this->outputFilename = other.outputFilename;
    this->backgroundColor = other.backgroundColor;
    this->cullingEnabled = other.cullingEnabled;
}

void Camera::setCameraTransformationMatrix() {
    cameraTransformationMatrix.values[0][0] = u.x;
    cameraTransformationMatrix.values[0][1] = u.y;
    cameraTransformationMatrix.values[0][2] = u.z;
    cameraTransformationMatrix.values[0][3] = -1 * u.dot(position);
    cameraTransformationMatrix.values[1][0] = v.x;
    cameraTransformationMatrix.values[1][1] = v.y;
    cameraTransformationMatrix.values[1][2] = v.z;
    cameraTransformationMatrix.values[1][3] = -1 * v.dot(position);
    cameraTransformationMatrix.values[2][0] = w.x;
    cameraTransformationMatrix.values[2][1] = w.y;
    cameraTransformationMatrix.values[2][2] = w.z;
    cameraTransformationMatrix.values[2][3] = -1 * w.dot(position);
    cameraTransformationMatrix.values[3][0] = 0;
    cameraTransformationMatrix.values[3][1] = 0;
    cameraTransformationMatrix.values[3][2] = 0;
    cameraTransformationMatrix.values[3][3] = 1;
}
void Camera::setProjectionTransformationMatrix() {
    if (projectionType == ORTOGRAPHIC_PROJECTION) {
        projectionTransformationMatrix.values[0][0] = 2 / (right - left);
        projectionTransformationMatrix.values[0][1] = 0;
        projectionTransformationMatrix.values[0][2] = 0;
        projectionTransformationMatrix.values[0][3] = -1 * (right + left) / (right - left);
        projectionTransformationMatrix.values[1][0] = 0;
        projectionTransformationMatrix.values[1][1] = 2 / (top - bottom);
        projectionTransformationMatrix.values[1][2] = 0;
        projectionTransformationMatrix.values[1][3] = -1 * (top + bottom) / (top - bottom);
        projectionTransformationMatrix.values[2][0] = 0;
        projectionTransformationMatrix.values[2][1] = 0;
        projectionTransformationMatrix.values[2][2] = 2 / (near - far);
        projectionTransformationMatrix.values[2][3] = -1 * (near + far) / (far - near);
        projectionTransformationMatrix.values[3][0] = 0;
        projectionTransformationMatrix.values[3][1] = 0;
        projectionTransformationMatrix.values[3][2] = 0;
        projectionTransformationMatrix.values[3][3] = 1;
    }
    else {
        projectionTransformationMatrix.values[0][0] = (2 * near) / (right - left);
        projectionTransformationMatrix.values[0][1] = 0;
        projectionTransformationMatrix.values[0][2] = (right + left) / (right - left);
        projectionTransformationMatrix.values[0][3] = 0;
        projectionTransformationMatrix.values[1][0] = 0;
        projectionTransformationMatrix.values[1][1] = (2 * near) / (top - bottom);
        projectionTransformationMatrix.values[1][2] = (top + bottom) / (top - bottom);
        projectionTransformationMatrix.values[1][3] = 0;
        projectionTransformationMatrix.values[2][0] = 0;
        projectionTransformationMatrix.values[2][1] = 0;
        projectionTransformationMatrix.values[2][2] = - (far + near) / (far - near);
        projectionTransformationMatrix.values[2][3] = - (2 * near * far) / (far - near);
        projectionTransformationMatrix.values[3][0] = 0;
        projectionTransformationMatrix.values[3][1] = 0;
        projectionTransformationMatrix.values[3][2] = -1;
        projectionTransformationMatrix.values[3][3] = 0;
    }
}
void Camera::setViewportTransformationMatrix() {
    viewportTransformationMatrix.values[0][0] = horRes / 2;
    viewportTransformationMatrix.values[0][1] = 0;
    viewportTransformationMatrix.values[0][2] = 0;
    viewportTransformationMatrix.values[0][3] = (horRes - 1) / 2;
    viewportTransformationMatrix.values[1][0] = 0;
    viewportTransformationMatrix.values[1][1] = verRes / 2;
    viewportTransformationMatrix.values[1][2] = 0;
    viewportTransformationMatrix.values[1][3] = (verRes - 1) / 2;
    viewportTransformationMatrix.values[2][0] = 0;
    viewportTransformationMatrix.values[2][1] = 0;
    viewportTransformationMatrix.values[2][2] = 0.5;
    viewportTransformationMatrix.values[2][3] = 0.5;
    viewportTransformationMatrix.values[3][0] = 0;
    viewportTransformationMatrix.values[3][1] = 0;
    viewportTransformationMatrix.values[3][2] = 0;
    viewportTransformationMatrix.values[3][3] = 1;
}

Vec4 Camera::getVertex(int index) {
    return vertices[index];
}

void Camera::applyCameraTransformation() {
    setCameraTransformationMatrix();
    for (auto& mesh : meshes) {
        for (auto& face : mesh.faces) {
            for (int i = 0; i < 3; i ++) {
                face.vertices[i] = cameraTransformationMatrix * face.vertices[i];
            }
        }
    }
}

void Camera::applyProjectionTransformation() {
    setProjectionTransformationMatrix();
   for (auto& mesh : meshes) {
        for (auto& face : mesh.faces) {
            for (int i = 0; i < 3; i ++) {
                face.vertices[i] = projectionTransformationMatrix * face.vertices[i];
                face.vertices[i] = face.vertices[i] / face.vertices[i].t;
            }
        }
    }
}

void Camera::applyViewportTransformation() {
    setViewportTransformationMatrix();
    for (auto& mesh : meshes) {
        for (auto& face : mesh.faces) {
            for (int i = 0; i < 3; i ++) {
                face.vertices[i] = viewportTransformationMatrix * face.vertices[i];
            }
        }
    }
}

void Camera::setMeshes(std::vector<Mesh*> meshes) {
    for (auto mesh : meshes) {
        Mesh newMesh;
        newMesh.meshId = mesh->meshId;
        newMesh.type = mesh->type;
        newMesh.numberOfTransformations = mesh->numberOfTransformations;
        newMesh.transformationIds = mesh->transformationIds;
        newMesh.transformationTypes = mesh->transformationTypes;
        newMesh.numberOfTriangles = mesh->numberOfTriangles;
        newMesh.triangles = mesh->triangles;
        newMesh.faces = mesh->faces;
        newMesh.modelingTransformationMatrix = mesh->modelingTransformationMatrix;
        this->meshes.push_back(newMesh);
    }
}

void Camera::primitiveAssembly(bool isCullingEnabled) {
    // primitives: points(done), lines, triangles
    for (auto mesh : meshes) {
        if (mesh.type == 0) { // wireframe
            for (auto face: mesh.faces) {
                if (isCullingEnabled and face.isBackface) {
                    continue;
                }
                this->lines.push_back(Line(face.vertices[0], face.vertices[1], face.colors[0], face.colors[1]));
                this->lines.push_back(Line(face.vertices[1], face.vertices[2], face.colors[1], face.colors[2]));
                this->lines.push_back(Line(face.vertices[2], face.vertices[0], face.colors[2], face.colors[0]));
            }
        }
        else { // solid
            for (auto face: mesh.faces) {
                if (isCullingEnabled and face.isBackface) {
                    continue;
                }
                this->triangles.push_back(face);

            }
        }
    }
}

void Camera::backfaceCulling() {
    for (auto& mesh: meshes)
    for (auto& triangle : mesh.faces) {
        triangle.calculateNormal();
        Vec3 view = Vec3(triangle.vertices[0].x, triangle.vertices[0].y, triangle.vertices[0].z);
        float dotProduct = triangle.normal.dot(view);
        if (dotProduct >= 0) {
            triangle.isBackface = false;
        }
        else {
            triangle.isBackface = true;
        }
    }
}

void Camera::render() {
    for (auto triangle: triangles) {
        drawTriangle(triangle.vertices[0].x, triangle.vertices[0].y,
                    triangle.vertices[1].x, triangle.vertices[1].y,
                    triangle.vertices[2].x, triangle.vertices[2].y,
                    triangle.vertices[0].z, triangle.vertices[1].z, triangle.vertices[2].z,
                    triangle.colors[0], triangle.colors[1], triangle.colors[2]);
    }
    for (auto line: lines) {
        drawLine(line.start.x, line.start.y, line.end.x, line.end.y, line.start.z, line.end.z, line.c1, line.c2);
    }
}

void Camera::drawLine(int x1, int y1, int x2, int y2, float z1, float z2, Color c1, Color c2) {
    if (x2 < x1) {
        std::swap(x1, x2);
        std::swap(y1, y2);
        std::swap(c1, c2);
        std::swap(z1, z2);
    }

    // if (!clipping(x1, y1, x2, y2, z1, z2, c1, c2, left, right, bottom, top)) return;

    // we assume that x1 is always <= x2
    int dx = x2 - x1; // always >= 0
    int dy = y2 - y1;

    int p = 0;

    int x = x1, y = y1;

    if (dy >= 0) {
        if (dx >= dy) {
            p = 2*dy - dx;
            while (x <= x2) {
                float t = x*1.0/(x2-x1);
                float z = z1*(1-t) + z2*t;
                drawPixel(x, y, z, calculateLineColor(c1, c2, t));

                if (p < 0) {
                    p += 2*dy;
                    x ++;
                }
                else { // P >= 0
                    p += 2*dy - 2*dx;
                    x ++;
                    y ++;
                }
            }
        }
        else { // dx < dy
            p = 2*dx - dy;
            while (y <= y2) {
                float t = y*1.0/(y2-y1);
                float z = z1*(1-t) + z2*t;
                drawPixel(x, y, z, calculateLineColor(c1, c2, t));

                if (p < 0) {
                    p += 2*dx;
                    y ++;
                }
                else { // P >= 0
                    p += 2*dx - 2*dy;
                    x ++;
                    y ++;
                }
            }
        }
    }
    else { // dy < 0
        dy = abs(dy);
        if (dx >= dy) {
            p = 2*dy - dx;
            while (x <= x2) {
                float t = x*1.0/(x2-x1);
                float z = z1*(1-t) + z2*t;
                drawPixel(x, y, z, calculateLineColor(c1, c2, t));

                if (p < 0) {
                    p += 2*dy;
                    x ++;
                }
                else { // P >= 0
                    p += 2*dy - 2*dx;
                    x ++;
                    y --;
                }
            }
        }
        else { // dx < dy
            p = 2*dx - dy;
            while (y >= y2) {
                float t = y*1.0/(y1-y2);
                float z = z1*(1-t) + z2*t;
                drawPixel(x, y, z, calculateLineColor(c1, c2, t));

                if (p < 0) {
                    p += 2*dx;
                    y --;
                }
                else { // P >= 0
                    p += 2*dx - 2*dy;
                    x ++;
                    y --;
                }
            }
        }
    }
}

Color Camera::calculateLineColor(Color c1, Color c2, float t) {
    return Color (
        c1.r * (1 - t) + c2.r * t,
        c1.g * (1 - t) + c2.g * t,
        c1.b * (1 - t) + c2.b * t
    );
}


void Camera::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                            float z1, float z2, float z3,
                            Color c1, Color c2, Color c3) {
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
        std::swap(c1, c2);
        std::swap(z1, z2);
    }
    if (x1 > x3) {
        std::swap(x1, x3);
        std::swap(y1, y3);
        std::swap(c1, c3);
        std::swap(z1, z3);
    }
    if (x2 > x3) {
        std::swap(x2, x3);
        std::swap(y2, y3);
        std::swap(c2, c3);
        std::swap(z2, z3);
    }
    // now we have x1 <= x2 <= x3

    // TODO clipping
    int minx = x1, maxx = x3;
    int miny = min(y1, min(y2, y3)), maxy = max(y1, max(y2, y3));

    for (int x = minx; x <= maxx; ++ x) {
        for (int y = miny; y <= maxy; ++ y) {
            if (isWithinImage(x, y) && 
                isInsideTriangle(x1, y1, x2, y2, x3, y3, x, y)) {
                // calculate barycentric coordinates
                float alpha = calculateLineEquation(x2, y2, x3, y3, x, y) * 1.0 / calculateLineEquation(x2, y2, x3, y3, x1, y1);
                float beta = calculateLineEquation(x3, y3, x1, y1, x, y) * 1.0 / calculateLineEquation(x3, y3, x1, y1, x2, y2);
                float gamma = calculateLineEquation(x1, y1, x2, y2, x, y) * 1.0 / calculateLineEquation(x1, y1, x2, y2, x3, y3);
                float d = alpha * z1 + beta * z2 + gamma * z3;
                drawPixel(x, y, d, calculateTriangleColor(c1, c2, c3, alpha, beta, gamma));
            }
        }
    }
}

bool Camera::isWithinImage(int x, int y) {
    return (x >= 0 && x < horRes && y >= 0 && y < verRes);
}

int Camera::calculateLineEquation(int x1, int y1, int x2, int y2, int x, int y) {
    return x * (y1 - y2) + y * (x2 - x1) + x1 * y2 - x2 * y1;
}

bool Camera::isInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
    int f12 = calculateLineEquation(x1, y1, x2, y2, x, y);
    int f23 = calculateLineEquation(x2, y2, x3, y3, x, y);
    int f31 = calculateLineEquation(x3, y3, x1, y1, x, y);
    return (f12 <= 0 && f23 <= 0 && f31 <= 0)  || (f12 >= 0 && f23 >= 0 && f31 >= 0);
}

Color Camera::calculateTriangleColor(Color c1, Color c2, Color c3, float t1, float t2, float t3) {
    return Color (
        c1.r * t1 + c2.r * t2 + c3.r * t3,
        c1.g * t1 + c2.g * t2 + c3.g * t3,
        c1.b * t1 + c2.b * t2 + c3.b * t3
    );
}

void Camera::drawPixel(int x, int y, float z, Color c) {
    if (isWithinImage(x, y)) {
        if (z < depth[x][y]) {
            // update the depth
            depth[x][y] = z;
            // color the pixel
            image[x][y] = c;
        }
    }
}

const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;

static int computeCode(double x, double y, double xmin, double xmax, double ymin, double ymax) {
    int code = 0;
    if (x < xmin)
        code |= LEFT;
    if (x > xmax)
        code |= RIGHT;
    if (y < ymin)
        code |= BOTTOM;
    if (y > ymax)
        code |= TOP;
    return code;
}

bool Camera::clipping(int& x1, int& y1, int& x2, int& y2, float& z1, float& z2, Color& c1, Color& c2,
                      int xmin, int xmax, int ymin, int ymax) {
    int code1, code2;
    double dx = x2 - x1, dy = y2 - y1;
    double tmin = 0, tmax = 1;

    for (int edge = LEFT; edge <= TOP; edge *= 2) {
        switch (edge) {
            case LEFT:   code1 = computeCode(x1, y1, xmin, xmax, ymin, ymax); break;
            case RIGHT:  code2 = computeCode(x2, y2, xmin, xmax, ymin, ymax); break;
            case BOTTOM: code1 = computeCode(x1, y1, xmin, xmax, ymin, ymax); break;
            case TOP:    code2 = computeCode(x2, y2, xmin, xmax, ymin, ymax); break;
        }

        if (code1 & code2) {
            // Trivial rejection: Both endpoints are outside the window.
            return false;
        } else {
            if (code1 == 0 && code2 == 0) {
                // Trivial acceptance: Both endpoints are inside the window.
                cout << "Line is completely visible." << endl;
                cout << "Endpoints: (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")" << endl;
                return true;
            } else {
                // Calculate intersection with the window and update tmin and tmax.
                if (code1 != 0) {
                    if (edge == LEFT) {
                        tmin = max(tmin, (xmin - x1) / dx);
                    } else if (edge == RIGHT) {
                        tmin = max(tmin, (xmax - x1) / dx);
                    } else if (edge == BOTTOM) {
                        tmin = max(tmin, (ymin - y1) / dy);
                    } else if (edge == TOP) {
                        tmin = max(tmin, (ymax - y1) / dy);
                    }
                } else {
                    if (edge == LEFT) {
                        tmax = min(tmax, (xmin - x1) / dx);
                    } else if (edge == RIGHT) {
                        tmax = min(tmax, (xmax - x1) / dx);
                    } else if (edge == BOTTOM) {
                        tmax = min(tmax, (ymin - y1) / dy);
                    } else if (edge == TOP) {
                        tmax = min(tmax, (ymax - y1) / dy);
                    }
                }
            }
        }
    }

    // After all edge tests, check if line segment is completely outside the window.
    if (tmin > tmax) {
        cout << "Line is completely outside the window." << endl;
        return false;
    } else {
        // Compute the new coordinates of the clipped line segment.
        x1 = x1 + tmin * dx;
        y1 = y1 + tmin * dy;
        x2 = x1 + tmax * dx;
        y2 = y1 + tmax * dy;
    }

    return true;
}



/////////////////// Image management functions


void Camera::assignColorToPixel(int i, int j, Color c)
{
    this->image[i][j].r = c.r;
    this->image[i][j].g = c.g;
    this->image[i][j].b = c.b;
}

/*
    Initializes image with background color
*/
void Camera::initializeImage()
{
    if (this->image.empty())
    {
        for (int i = 0; i < horRes; i++)
        {
            vector<Color> rowOfColors;
            vector<double> rowOfDepths;

            for (int j = 0; j < verRes; j++)
            {
                rowOfColors.push_back(this->backgroundColor);
                rowOfDepths.push_back(far);
            }

            this->image.push_back(rowOfColors);
            this->depth.push_back(rowOfDepths);
        }
    }
    else
    {
        for (int i = 0; i < horRes; i++)
        {
            for (int j = 0; j < verRes; j++)
            {
                assignColorToPixel(i, j, this->backgroundColor);
                this->depth[i][j] = far;
            }
        }
    }
}

/*
    If given value is less than 0, converts value to 0.
    If given value is more than 255, converts value to 255.
    Otherwise returns value itself.
*/
int Camera::makeBetweenZeroAnd255(double value)
{
    if (value >= 255.0)
        return 255;
    if (value <= 0.0)
        return 0;
    return (int)(value);
}

/*
    Writes contents of image (Color**) into a PPM file.
*/
void Camera::writeImageToPPMFile()
{
    ofstream fout;

    fout.open(outputFilename.c_str());

    fout << "P3" << endl;
    fout << "# " << outputFilename << endl;
    fout << horRes << " " << verRes << endl;
    fout << "255" << endl;

    for (int j = verRes - 1; j >= 0; j--)
    {
        for (int i = 0; i < horRes; i++)
        {
            fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
        }
        fout << endl;
    }
    fout.close();
}

/*
    Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
*/
void Camera::convertPPMToPNG(string ppmFileName)
{
    string command;

    // TODO: Change implementation if necessary.
    command = "./magick convert " + ppmFileName + " " + ppmFileName + ".png";
    system(command.c_str());
}


//////////////////// Stream output

std::ostream &operator<<(std::ostream &os, const Camera &c)
{
    const char *camType = c.projectionType ? "perspective" : "orthographic";

    os << std::fixed << std::setprecision(6) << "Camera " << c.cameraId << " (" << camType << ") => pos: " << c.position << " gaze: " << c.gaze << std::endl
       << "\tu: " << c.u << " v: " << c.v << " w: " << c.w << std::endl
       << std::fixed << std::setprecision(3) << "\tleft: " << c.left << " right: " << c.right << " bottom: " << c.bottom << " top: " << c.top << std::endl
       << "\tnear: " << c.near << " far: " << c.far << " resolutions: " << c.horRes << "x" << c.verRes << " fileName: " << c.outputFilename;

    return os;
}

