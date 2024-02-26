#ifndef LINE_H
#define LINE_H

#include "Vec4.h"
#include "Color.h"

class Line {
public:
    Vec4 start, end;
    Color c1, c2;
    Line();
    Line(Vec4 start, Vec4 end, Color c1, Color c2);

    friend std::ostream &operator<<(std::ostream &os, const Line &line);
};

#endif //LINE_H