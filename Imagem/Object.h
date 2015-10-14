#ifndef _Object_H_
#define _Object_H_

#include <vector>
#include "math/Point.h"
#include "Ray.h"
#include "Sphere.h"
#include "math/Vec3.h"
#include "triangle.h"
#include "Intersect.h"


class Object : public Primitive {
    public:
        Object();

        virtual Vec3 getNormal() {};
        virtual Vec3 getNormal(const Point& p) { }

        bool Intersect(const Ray& r, std::list<Interval> &interval);

    //private:
        std::vector<Triangle*> triangles;
        Sphere box;
};

#endif
