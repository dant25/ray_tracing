#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "math/Point.h"
#include "math/Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "BoundingBox.h"
#include "Intersect.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <list>
#include "Color.h"

enum PrimitiveType { SPHERE, TRIANGLE};
class Primitive {
    public:
        //TODO desconsiderar material
        Primitive();
        Primitive(const Material& mat);

        PrimitiveType getPrimitiveType() {return type;};

        virtual bool Intersect(const Ray& ray, std::list<Interval> &interval) = 0;

        virtual Vec3 getNormal(const Point& p) = 0;
        virtual Vec3 getNormal() = 0;
    
        int id;
    //protected:
        Material material;
        //glm::mat4 transform;
        BoundingBox boundingBox;
    protected:
        PrimitiveType type;

};

#endif
