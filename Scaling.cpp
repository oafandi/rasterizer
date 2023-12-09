#include <iomanip>
#include "Scaling.h"

Scaling::Scaling() {
    this->scalingId = -1;
    this->sx = 0;
    this->sy = 0;
    this->sz = 0;
}

Scaling::Scaling(int scalingId, double sx, double sy, double sz)
{
    this->scalingId = scalingId;
    matrix.values[0][0] = sx;
    matrix.values[0][1] = 0;
    matrix.values[0][2] = 0;
    matrix.values[0][3] = 0;
    matrix.values[1][0] = 0;
    matrix.values[1][1] = sy;
    matrix.values[1][2] = 0;
    matrix.values[1][3] = 0;
    matrix.values[2][0] = 0;
    matrix.values[2][1] = 0;
    matrix.values[2][2] = sz;
    matrix.values[2][3] = 0;
    matrix.values[3][0] = 0;
    matrix.values[3][1] = 0;
    matrix.values[3][2] = 0;
    matrix.values[3][3] = 1;
}

std::ostream &operator<<(std::ostream &os, const Scaling &s)
{
    os << std::fixed << std::setprecision(3) << "Scaling " << s.scalingId << " => [" << s.sx << ", " << s.sy << ", " << s.sz << "]";

    return os;
}
