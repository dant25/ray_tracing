#include "point3d.h"

Point3D::Point3D(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Point3D::Point3D(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3D::Point3D(double x, double z){
    this->x = x;
    this->y = 0;
    this->z = z;
}

void Point3D::setX(double x){this->x = x;    }
void Point3D::setY(double y){this->y = y;    }
void Point3D::setZ(double z){this->z = z;    }
void Point3D::setId(int id) {this->id = id;  }
void Point3D::setXYZ(double x, double y, double z){this->x = x; this->y = y; this->z = z;}

double Point3D::getX()      {return this->x; }
double Point3D::getY()      {return this->y; }
double Point3D::getZ()      {return this->z; }
int    Point3D::getId()     {return this->id;}

Point3D Point3D::operator+(Point3D p){
    Point3D plus;
    plus.setX(this->getX() + p.getX());
    plus.setY(this->getY() + p.getY());
    plus.setZ(this->getZ() + p.getZ());
    return plus;
}

Point3D Point3D::operator-(Point3D p){
    Point3D minus;
    minus.setX(this->getX() - p.getX());
    minus.setY(this->getY() - p.getY());
    minus.setZ(this->getZ() - p.getZ());
    return minus;
}

Point3D Point3D::operator/(int d){
    Point3D div;
    div.setX(this->getX()/d);
    div.setY(this->getY()/d);
    div.setZ(this->getZ()/d);
    return div;
}

Point3D Point3D::operator*(float m){
    Point3D mul;
    mul.setX(this->getX()*m);
    mul.setY(this->getY()*m);
    mul.setZ(this->getZ()*m);
    return mul;
}

bool Point3D::operator==(Point3D p){
    if(this->getX() == p.getX() &&
       this->getY() == p.getY() &&
       this->getZ() == p.getZ()   )
    {
        return true;
    }
    return false;
}

bool Point3D::operator!=(Point3D p){
    if(this->getX() != p.getX() ||
       this->getY() != p.getY() ||
       this->getZ() != p.getZ()   )
    {
        return true;
    }
    return false;
}




double Point3D::dot(Point3D p1, Point3D p2){
    double dot = p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
    return dot;
}

double Point3D::module(Point3D p){
    return pow(p.getX()*p.getX() + p.getY()*p.getY() + p.getZ()*p.getZ(),0.5);
}

Point3D Point3D::normalize(Point3D p){
    float den = module(p);
    Point3D norm = Point3D(p.getX()/den,
                           p.getY()/den,
                           p.getZ()/den);
    return norm;
}

float Point3D::angle(Point3D p1, Point3D p2){
    float PI = 3.14159265;
    return (180.0/PI) * acosf( dot(p1,p2) / (module(p1)*module(p2)) );
}

Point3D Point3D::cross(Point3D p1, Point3D p2){
    Point3D cross = Point3D(p1.getY()*p2.getZ() - p1.getZ()*p2.getY(),
                            p1.getZ()*p2.getX() - p1.getX()*p2.getZ(),
                            p1.getX()*p2.getY() - p1.getY()*p2.getX());
    return cross;
}

float Point3D::distance(Point3D p1, Point3D p2){
    return pow( (pow(p2.getX()-p1.getX(),2) + pow(p2.getY()-p1.getY(),2) + pow(p2.getZ()-p1.getZ(),2)) ,0.5);
}

