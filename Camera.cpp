#include <iomanip>
#include "Camera.h"
#include "Vec4.h"

Camera::Camera() {}

Camera::Camera(int cameraId,
               int projectionType,
               Vec3 position, Vec3 gaze,
               Vec3 u, Vec3 v, Vec3 w,
               double left, double right, double bottom, double top,
               double near, double far,
               int horRes, int verRes,
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
}

void Camera::setVertices(std::vector<Vec4> &vertices) {
    for (int i = 0; i < vertices.size(); i ++) {
        this->vertices.push_back(vertices[i]);
    }
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
        projectionTransformationMatrix.values[2][2] = (near + far) / (near - far);
        projectionTransformationMatrix.values[2][3] = (2 * near * far) / (near - far);
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

void Camera::applyCameraTransformation() {
    for (int i = 0; i < vertices.size(); i ++) {
        vertices[i] = cameraTransformationMatrix * vertices[i];
    }
}

void Camera::applyProjectionTransformation() {
    for (int i = 0; i < vertices.size(); i ++) {
        vertices[i] = projectionTransformationMatrix * vertices[i];
        vertices[i] = vertices[i] / vertices[i].t;
    }
}

void Camera::applyViewportTransformation() {
    for (int i = 0; i < vertices.size(); i ++) {
        
    }
}

std::ostream &operator<<(std::ostream &os, const Camera &c)
{
    const char *camType = c.projectionType ? "perspective" : "orthographic";

    os << std::fixed << std::setprecision(6) << "Camera " << c.cameraId << " (" << camType << ") => pos: " << c.position << " gaze: " << c.gaze << std::endl
       << "\tu: " << c.u << " v: " << c.v << " w: " << c.w << std::endl
       << std::fixed << std::setprecision(3) << "\tleft: " << c.left << " right: " << c.right << " bottom: " << c.bottom << " top: " << c.top << std::endl
       << "\tnear: " << c.near << " far: " << c.far << " resolutions: " << c.horRes << "x" << c.verRes << " fileName: " << c.outputFilename;

    return os;
}