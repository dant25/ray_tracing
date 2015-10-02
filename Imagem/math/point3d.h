#ifndef POINT3D_H
#define POINT3D_H

#include <math.h>

class Point3D
{
public:
    //constructors
    Point3D ();
    Point3D (double x, double y, double z);
    Point3D (double x, double z);

    //setters and getters methods
    void    setX     (double  x);
    void    setY     (double  y);
    void    setZ     (double  z);
    void    setId    (int    id);
    void    setXYZ   (double x, double y, double z);

    double  getX();
    double  getY();
    double  getZ();
    int     getId();

    //simple operators
    Point3D operator+(Point3D p);
    Point3D operator-(Point3D p);
    Point3D operator/(int     d);
    Point3D operator*(float   m);
    bool    operator==(Point3D p);
    bool    operator!=(Point3D p);

    //complex operators
    static double  dot      (Point3D p1, Point3D p2);
    static double  module   (Point3D p);
    static Point3D normalize(Point3D  p);
    static Point3D cross    (Point3D p1, Point3D p2);

    static float angle(Point3D p1, Point3D p2);

    static float distance(Point3D p1, Point3D p2);

    double x,y,z;  //let y be the height.
    int       id;
};

#endif // POINT3D_H
