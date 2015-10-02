#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "math/Point.h"

class BoundingBox {
public:
    BoundingBox() { };
    BoundingBox(const Point& pMin, const Point& pMax) {
        this->pMin = pMin;
        this->pMax = pMax;
    }

    Point pMin, pMax;

};

#endif

