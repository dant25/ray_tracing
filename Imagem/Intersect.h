#ifndef _INTERSECTION_H_
#define _INTERSECTION_H_

#include "math/Point.h"
#include "math/Vec3.h"
#include "Material.h"


struct Intersection {
    //FIXME talvez dist não seja necessário
    float dist;
    Point point;
    Vec3 normal;
    Material material;
    int objIndex;
};

struct Interval {
    Intersection begin;
    Intersection end;
};

#endif
