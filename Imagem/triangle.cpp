#include "triangle.h"
#include <cmath>
#include <iostream>

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3) : p1(p1), p2(p2), p3(p3)
{
    this->norm = getNormal();
}

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3, Material mat) : p1(p1), p2(p2), p3(p3)
{
    this->material = mat;

    this->norm = getNormal();
}

Vec3 Triangle::getNormal()
{
    Vec3 v1 = p2-p1;
    Vec3 v2 = p3-p1;

     Vec3 norm = Cross(v1, v2);

     norm.normalize();

     return norm;
}

bool Triangle::Intersect(const Ray& r, std::list<Interval> &interval)
{
    Ray ray = r;
    float valueA = Dot( ray.d, this->norm);
    float valueB = Dot( (p1 - r.o), this->norm);

    if( fabs(valueA) <= 0.00001 )
            return false;

    float T = valueB / valueA;

    if( T <= 0.0 )
        return false;


    ///VERIFICA PONTO QUE ESTÁ NO PLANO
    Point P = r( T );
//std::cout << P.x << "\t" << P.y << "\t" << P.z << std::endl;


    ///VERIFICA SE ESTÁ DENTRO DO TRIANGULO
    Vec3 u = p2 - p1;
    Vec3 v = p3 - p1;

    float    uu, uv, vv, wu, wv, D;
    uu = Dot(u,u);
    uv = Dot(u,v);
    vv = Dot(v,v);

    Vec3 w = P - p1;
    wu = Dot(w,u);
    wv = Dot(w,v);
    D = uv * uv - uu * vv;

    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;

    ///COLOCANDO INFORMAÇÕES DO TRIANGULO NA ESTRUTURA
    Intersection intersection;
    intersection.material = this->material;
    intersection.normal = this->norm;
    intersection.objIndex = this->id;
    intersection.point = P;
    intersection.dist = (intersection.point - r.o).length();

    Interval i;
    i.begin = intersection;
    i.end = intersection;
    interval.push_back(i);

    return true;
}

