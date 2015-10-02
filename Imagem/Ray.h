#ifndef _RAY_H_
#define _RAY_H_

#include "math/Point.h"
#include "math/Vec3.h"

class Ray {
    public:
        Ray() {};
        Ray(const Point& p, const Vec3& v)
                : o(p), d (v) {
            o = p;
            d = v;
        }

        Point operator()(float t) const{
            return o + d*t;
        }

        Vec3 raio() {
            Vec3 raio;
            raio = (o-d);
            raio.normalize();
            return raio;
        }

        Point o;
        Vec3 d;
        int IDfonte = -1;
};

#endif
