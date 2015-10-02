#ifndef QUATERNIONS_H
#define QUATERNIONS_H


#include "point3d.h"

typedef struct Quaternion{
    double x;
    double y;
    double z;
    double w;
}quat;

quat Multq(const quat q2, const quat q1) {
    quat temp;
    temp.x = q2.y*q1.z - q2.z*q1.y + q2.w*q1.x + q2.x*q1.w;
    temp.y = q2.z*q1.x - q2.x*q1.z + q2.w*q1.y + q2.y*q1.w;
    temp.z = q2.x*q1.y - q2.y*q1.x + q2.w*q1.z + q2.z*q1.w;
    temp.w = q2.w*q1.w - q2.x*q1.x - q2.y*q1.y - q2.z*q1.z;
    return temp;
}


quat Multqv(const quat q, const Point3D v) {
    quat res;
    res.w = -(q.x*v.x) - (q.y* v.y) - (q.z*v.z);
    res.x = (q.w*v.x) + (q.y*v.z) - (q.z*v.y);
    res.y = (q.w*v.y) + (q.z*v.x) - (q.x*v.z);
    res.z = (q.w*v.z) + (q.x*v.y) - (q.y*v.x);

    return res;
}


Point3D rotateQuat(Point3D vecToRotate, Point3D axis, float angle){

    float radAngle = (angle * 0.5)* 3.1456/180;
    Point3D::normalize(axis);

    quat q;
    q.x = axis.x * sin(radAngle);
    q.y = axis.y * sin(radAngle);
    q.z = axis.z * sin(radAngle);
    q.w = cos(radAngle);

    quat qConjugate;
    qConjugate.x = -q.x,
    qConjugate.y = -q.y,
    qConjugate.z = -q.z;
    qConjugate.w = q.w;


    quat qTemp, qRes;

    qTemp = Multqv(q, vecToRotate);
    qRes = Multq(qTemp, qConjugate);

    Point3D res = Point3D(qRes.x,qRes.y,qRes.z);

    return res;
}


#endif // QUATERNIONS_H
