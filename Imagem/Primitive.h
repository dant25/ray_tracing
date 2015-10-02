#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "math/Point.h"
#include "math/Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "BoundingBox.h"
#include "Intersect.h"

#include <list>
#include "Color.h"

enum PrimitiveType { CUBE, CORNER, CYLINDER, SPHERE};
class Primitive {
    public:
        //TODO desconsiderar material
        Primitive();
        Primitive(const Material& mat);

        virtual PrimitiveType getPrimitiveType() = 0;

        virtual bool Intersect(const Ray& ray, std::list<Interval> &interval) = 0;
        virtual Vec3 getNormal(const Point& p) = 0;
    
        int id;
    //protected:
        Material material;
        BoundingBox boundingBox;
    protected:
        PrimitiveType type;

};

#endif
