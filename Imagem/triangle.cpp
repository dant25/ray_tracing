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
/*
bool Triangle::Intersect(const Ray& r, std::list<Interval> &interval)
{
    float kEpsilon = 0.000001;

    // Step 1: finding P
    //check if ray and plane are parallel
    float NdotRayDirection = Dot( this->norm, r.d);
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0
        return false; // they are parallel so they don't intersect !

     //compute the parameter D on equation
    float d = Dot(this->norm, p1);

    //compute t
    float t;
    float den = Dot(this->norm, r.d );
    t = Dot(p1, this->norm) - Dot(r.o, this->norm);
    t /= den;

    // check if the triangle is in behind the ray
    if (t < 0)
        return false; // the triangle is behind

    //compute the intersection point using t
    Point P = r(t);

    // Step 2: inside-outside test
    Vec3 C; // vector perpendicular to triangle's plane

    //first edge
    Vec3 edge1 = p2 - p1;
    Vec3 vp1 = P - p1;
    C = Cross(edge1,vp1);
    if(Dot(this->norm,C) < 0 ) return false;

    //second edge
    Vec3 edge2 = p3 - p2;
    Vec3 vp2 = P - p2;
    C = Cross(edge2,vp2);
    if(Dot(this->norm,C) < 0) return false;

    //third edge
    Vec3 edge3 = p1 - p3;
    Vec3 vp3 = P - p3;
    C = Cross(edge3,vp3);
    if(Dot(this->norm,C) < 0) return false;



    ///COLOCANDO INFORMAÇÕES DO TRIANGULO NA ESTRUTURA
    Intersection intersection;
    intersection.material = this->material;
    intersection.normal = this->norm;
    intersection.vertexes_norm = this->vertexes_norm;
    intersection.objIndex = this->id;
    intersection.point = P;
    intersection.dist = (intersection.point - r.o).length();

    Interval i;
    i.begin = intersection;
    i.end = intersection;
    interval.push_back(i);

    return true;
}
*/

bool Triangle::Intersect(const Ray& r, std::list<Interval> &interval)
{
    Ray ray = r;
    double valueA = Dot( ray.d, this->norm);
    double valueB = Dot( (p1 - r.o), this->norm);

    if( fabs(valueA) <= 0.0001 )
            return false;

    float T = valueB / valueA;

    if( T <= 0.0001 )
        return false;


    ///VERIFICA PONTO QUE ESTÁ NO PLANO
    Point P = r( T );
//std::cout << P.x << "\t" << P.y << "\t" << P.z << std::endl;


    ///VERIFICA SE ESTÁ DENTRO DO TRIANGULO
    Vec3 u = p2 - p1;
    Vec3 v = p3 - p1;

    double uu, uv, vv, wu, wv, D;
    uu = Dot(u,u);
    uv = Dot(u,v);
    vv = Dot(v,v);

    Vec3 w = P - p1;
    wu = Dot(w,u);
    wv = Dot(w,v);
    D = uv * uv - uu * vv;

    double s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;


    ///CALCULO DA NORMAL NO VÉRTEX
    double v1,v2,v3;
    double dist1, dist2;
    double A, a,b,c;

    Vec3 aux1 = (this->p2 - this->p1);
    Vec3 aux2 = (this->p3 - this->p1);

    dist1 = aux1.length();
    dist2 = aux2.length();

    aux1.normalize();
    aux2.normalize();

    double ang = acos( Dot(aux1,aux2) );

    A = ( dist1*dist2*sin(ang) ) / 2.0;
    ///------------------------------------------------------------------------
    aux1 = (this->p2 - P);
    aux2 = (this->p3 - P);

    dist1 = aux1.length();
    dist2 = aux2.length();

    aux1.normalize();
    aux2.normalize();

    ang = acos( Dot(aux1,aux2) );

    a = ( dist1*dist2*sin(ang) ) / (2.0*A);     ///Coordenada do primeiro triangulo
    ///------------------------------------------------------------------------
    aux1 = (this->p3 - P);
    aux2 = (this->p1 - P);

    dist1 = aux1.length();
    dist2 = aux2.length();

    aux1.normalize();
    aux2.normalize();

    ang = acos( Dot(aux1,aux2) );

    b = ( dist1*dist2*sin(ang) ) / (2.0*A);     ///Coordenada do segundo triangulo
    ///------------------------------------------------------------------------
    aux1 = (this->p1 - P);
    aux2 = (this->p2 - P);

    dist1 = aux1.length();
    dist2 = aux2.length();

    aux1.normalize();
    aux2.normalize();

    ang = acos( Dot(aux1,aux2) );

    c = ( dist1*dist2*sin(ang) ) / (2.0*A);     ///Coordenada do terceiro triangulo
    ///------------------------------------------------------------------------


    ///COLOCANDO INFORMAÇÕES DO TRIANGULO NA ESTRUTURA
    Intersection intersection;

    if( a+b+c <= 1.00001 || a+b+c >= 0.99999 )
    {
    //std::cout << "Area T: " << A << std::endl;
    //std::cout << "Coordenadas: " << a << "  " << b << "  " << c << std::endl;
    //std::cout << "Sum: " << total << std::endl;
    intersection.vertexes_norm = (n1*a) + (n2*b) + (n3*c);
    }
    else
        intersection.vertexes_norm  = this->norm;


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



