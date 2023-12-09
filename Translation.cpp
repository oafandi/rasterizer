#include <iomanip>
#include "Translation.h"

Translation::Translation()
{
    this->translationId = -1;
    
    matrix.values[0][0] = 1.0;
    matrix.values[0][1] = 0.0;
    matrix.values[0][2] = 0.0;
    matrix.values[0][3] = 0.0;
    matrix.values[1][0] = 0.0;
    matrix.values[1][1] = 1.0;
    matrix.values[1][2] = 0.0;
    matrix.values[1][3] = 0.0;
    matrix.values[2][0] = 0.0;
    matrix.values[2][1] = 0.0;
    matrix.values[2][2] = 1.0;
    matrix.values[2][3] = 0.0;
    matrix.values[3][0] = 0.0;
    matrix.values[3][1] = 0.0;
    matrix.values[3][2] = 0.0;
    matrix.values[3][3] = 1.0;
}

Translation::Translation(int translationId, double tx, double ty, double tz)
{
    this->translationId = translationId;

    matrix.values[0][0] = 1.0;
    matrix.values[0][1] = 0.0;
    matrix.values[0][2] = 0.0;
    matrix.values[0][3] = tx;
    matrix.values[1][0] = 0.0;
    matrix.values[1][1] = 1.0;
    matrix.values[1][2] = 0.0;
    matrix.values[1][3] = ty;
    matrix.values[2][0] = 0.0;
    matrix.values[2][1] = 0.0;
    matrix.values[2][2] = 1.0;
    matrix.values[2][3] = tz;
    matrix.values[3][0] = 0.0;
    matrix.values[3][1] = 0.0;
    matrix.values[3][2] = 0.0;
    matrix.values[3][3] = 1.0;
}

std::ostream &operator<<(std::ostream &os, const Translation &t)
{
    os << std::fixed << std::setprecision(3) << "Translation " << t.translationId << " => [" << t.tx << ", " << t.ty << ", " << t.tz << "]";
    return os;
}