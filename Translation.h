#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "Matrix4.h"

class Translation
{
public:
    int translationId;
    Matrix4 matrix;

    Translation();
    Translation(int translationId, double tx, double ty, double tz);
    friend std::ostream &operator<<(std::ostream &os, const Translation &t);
};

#endif

