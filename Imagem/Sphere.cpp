#include "Sphere.h"
#include <cmath>
#include <iostream>

/*Sphere::Sphere( const Material& mat, const Point& p, float r) : Primitive(mat){
    center = p;
    radius = r;
    type = SPHERE;
}*/

Sphere::Sphere(const Point& center, float radius) : center(center), radius(radius) {

}

bool Sphere::Intersect(const Ray& ray, std::list<Interval> &interval) {
    //Vetor auxiliar gerado pelos pontos origem do raio
    //e centro da esfera

    Vec3 c = Vec3(center.x, center.y, center.z);
    Vec3 origin = Vec3(ray.o.x, ray.o.y, ray.o.z);
    float eqa = Dot(ray.d, ray.d);

//std::cout << ray.d.x << " " << ray.d.y << std::endl;
    float eqb = 2*(Dot(origin, ray.d) - Dot(ray.d, c) );
    float eqc = Dot(c, c) - Dot(c, origin)*2 + Dot(origin, origin) - radius*radius;

    float delta = eqb*eqb - 4*eqa*eqc;
    if (delta < 0) { //não há raízes reais
//std::cout << "sem raízes reais" << std::endl;
        return false;
    }
    else if (delta == 0) { //interseção somente em um ponto
//std::cout << "interseção em um ponto" << std::endl;
        Intersection intersectPoint;
        intersectPoint.dist = -eqb/(2*eqa);
        intersectPoint.point = ray.o + ray.d*intersectPoint.dist;
        intersectPoint.material = this->material;
        intersectPoint.normal = this->getNormal(intersectPoint.point);
        intersectPoint.vertexes_norm  = this->getNormal(intersectPoint.point);
        intersectPoint.objIndex = this->id;
        //O único ponto de interseção será o começo e o fim do intervalo
        Interval i;
        i.begin = intersectPoint;
        i.end = intersectPoint;
        interval.push_back(i);

        return (intersectPoint.dist > 0);
    }
    else { //delta > 0; duas raízes reais
//std::cout << "dua raízes reais" << std::endl;
        float sqrtDelta = sqrtf(delta);
        float t0 = (-eqb - sqrtDelta)/(2*eqa);
        float t1 = (-eqb + sqrtDelta)/(2*eqa);

        float smallest = (t0 < t1)?t0:t1;
        float largest = (t0 > t1)?t0:t1;

        //TODO  não tratamos o caso em que a câmera está dentro da esfera (smallest < 0
        //      e largest > 0)
        Intersection intsctPt1, intsctPt2;
        intsctPt2.dist = largest;
        intsctPt2.point = ray.o + ray.d*intsctPt2.dist;
        intsctPt2.normal = this->getNormal(intsctPt2.point);
        intsctPt2.vertexes_norm  = this->getNormal(intsctPt2.point);
        intsctPt2.material = this->material;
        intsctPt2.objIndex = this->id;

        intsctPt1.dist = smallest;
        intsctPt1.point = ray.o + ray.d*intsctPt1.dist;
        intsctPt1.normal = this->getNormal(intsctPt1.point);
        intsctPt1.vertexes_norm  = this->getNormal(intsctPt1.point);
        intsctPt1.material = this->material;
        intsctPt1.objIndex = this->id;
        Interval i;
        i.begin = intsctPt1;
        i.end = intsctPt2;
        interval.push_back(i);

        /*if(smallest < 0) {
            intsctPt1.dist = smallest;
            intsctPt1.point = ray.o + ray.d*intsctPt1.dist;
            intsctPt1.normal = this->getNormal(intsctPt1.point);
            intsctPt1.material = this->material;
            intsctPt1.objIndex = this->id;
        }
        else {
            intsctPt2.dist = largest;
            intsctPt2.point = ray.o + ray.d*intsctPt2.dist;
            intsctPt2.normal = this->getNormal(intsctPt2.point);
            intsctPt2.material = this->material;
            intsctPt2.objIndex = this->id;
        }
        if(intsctPt1.dist < 0)
            return false;*/
        if(largest < 0)
            return false;
        else
            return true;
    }
}
