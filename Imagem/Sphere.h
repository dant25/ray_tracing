#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <list>
#include "math/Point.h"
#include "Ray.h"
#include "math/Vec3.h"
#include "Primitive.h"
#include "Intersect.h"


class Sphere : public Primitive {
    public:
        //Sphere(const Material& mat, const Point& p, float r);
        Sphere(const Point& center, float radius);

        Vec3 getNormal(const Point& p) { return (p - center)/radius; }
        bool Intersect(const Ray& r, std::list<Interval> &interval);

        virtual Vec3 getNormal() {};


        float getRadius() { return radius; }
        Point& getCenter() { return center; }
    private:
        float radius;
        Point center;
};

#endif
