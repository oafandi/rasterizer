#include <iomanip>
#include "Rotation.h"

Rotation::Rotation() {
    this->rotationId = -1;
}

Rotation::Rotation(int rotationId, double angle, double x, double y, double z)
{
    this->rotationId = rotationId;
    // TODO: Implement this
}

std::ostream &operator<<(std::ostream &os, const Rotation &r)
{
    os << std::fixed << std::setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.ux << ", " << r.uy << ", " << r.uz << "]";
    return os;
}