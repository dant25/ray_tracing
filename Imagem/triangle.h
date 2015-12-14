#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <list>
#include "math/Point.h"
#include "Ray.h"
#include "math/Vec3.h"
#include "Primitive.h"
#include "Intersect.h"


class Triangle : public Primitive {
    public:
        Triangle(const Point& p1, const Point& p2, const Point& p3);
        Triangle(const Point& p1, const Point& p2, const Point& p3, Material mat);

        virtual bool Intersect(const Ray& r, std::list<Interval> &interval);

        virtual Vec3 getNormal(const Point& p) {return norm;};
        virtual Vec3 getNormal();

        void setNormal(Vec3 n) { this->norm = n; };

    //private:
        Point p1, p2, p3;
        Vec3 n1, n2, n3;
        Vec3 norm;
        //Vec3 vertexes_norm;
};

#endif // TRIANGLE_H
