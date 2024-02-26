#include "Line.h"
#include "Vec4.h"
#include <ostream>
#include <iomanip>

Line::Line() {
    this->start = Vec4();
    this->end = Vec4();   
}

Line::Line(Vec4 start, Vec4 end, Color c1, Color c2) {
    this->start = start;
    this->end = end;
    this->c1 = c1;
    this->c2 = c2;
}

std::ostream &operator<<(std::ostream &os, const Line &line)
{
    os << std::fixed << std::setprecision(0) << "Line with vertices (" << line.start << ", " << line.end << ")";
    return os;
}