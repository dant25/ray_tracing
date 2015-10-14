#include<iostream>
#include "Camera.h"
#include "Ray.h"
#include "Primitive.h"
#include "math/Quaternions.h"


Camera::Camera() :  ic(Vec3(1.0, 0.0, 0.0)), jc(Vec3(0.0, 1.0, 0.0)),
                    kc(Vec3(0.0, 0.0, 1.0)), pos(0.0, 0.0, 17.0)
{
    //Inicializa imagem
    //imgWidth = 500;
    //imgHeight = 500;
}

Camera::Camera(int width, int height)
{
    //Inicializa imagem
    imgWidth = width;
    imgHeight = height;

    ic = Point(1.0, 0.0, 0.0);
    jc = (Point(0.0, -1.0, 0.0));
    kc = (Point(0.0, 0.0, 1.0));
}

Camera::~Camera() {

}

Ray Camera::createRay(int wImg, int hImg) {
    float xw = 2*(wImg - imgWidth/2.0) / imgWidth;
    float yw = 2*(hImg - imgHeight/2.0) / imgHeight;

    //std::cout << wImg << " " << xw << "\t"<< hImg << " " << yw << std::endl;

    //ponto projetado
    Point p = pos + kc*(-1.0) + ic*xw + jc*yw;
    Vec3 dir = p - pos;
    dir.normalize();
    //Inicializa o raio
    return Ray(pos, dir);
}

Point Camera::getPos() {
    return this->pos;
}

void Camera::setPos(Point pos) {
    this->pos = pos;
}

Vec3 Camera::getLookAt()
{
    return this->kc;
}

void Camera::lookAt(Point point )
{
    this->kc = this->pos - point;
    this->kc.normalize();

    this->ic = Cross( ( ( point + Point(0, 1.0, 0) ) - this->pos  ), this->kc );
    this->ic.normalize();

    this->jc = Cross( this->ic, this->kc  );        ///TA INVERTIDO!!!! GRADE VIRA O VETOR

//    std::cout << ic.x << "\t" << ic.y << "\t" << ic.z << std::endl;
//    std::cout << jc.x << "\t" << jc.y << "\t" << jc.z << std::endl;
//    std::cout << kc.x << "\t" << kc.y << "\t" << kc.z << std::endl;
}

void Camera::Rotation_i(float ang)
{
    Point3D newVIEW, newUP;

    newUP = rotateQuat( Point3D(this->jc.x, this->jc.y, this->jc.z) , Point3D(this->ic.x, this->ic.y, this->ic.z), ang);
    newUP = Point3D::normalize(newUP);
    newVIEW = rotateQuat( Point3D(this->kc.x, this->kc.y, this->kc.z) , Point3D(this->ic.x, this->ic.y, this->ic.z), ang);
    newVIEW = Point3D::normalize(newVIEW);

    this->jc = Vec3(newUP.x, newUP.y, newUP.z);
    this->kc = Vec3(newVIEW.x, newVIEW.y, newVIEW.z);
}


void Camera::Rotation_j(float ang)
{
    Point3D newVIEW, newRIGHT;

    newVIEW = rotateQuat( Point3D(this->kc.x, this->kc.y, this->kc.z) , Point3D(this->jc.x, this->jc.y, this->jc.z), ang);
    newVIEW = Point3D::normalize(newVIEW);
    newRIGHT = rotateQuat( Point3D(this->ic.x, this->ic.y, this->ic.z) , Point3D(this->jc.x, this->jc.y, this->jc.z), ang);
    newRIGHT = Point3D::normalize(newRIGHT);

    this->ic = Vec3(newRIGHT.x, newRIGHT.y, newRIGHT.z);
    this->kc = Vec3(newVIEW.x, newVIEW.y, newVIEW.z);
}


void Camera::Rotation_k(float ang)
{
    Point3D newUP, newRIGHT;

    newUP = rotateQuat( Point3D(this->jc.x, this->jc.y, this->jc.z) , Point3D(this->kc.x, this->kc.y, this->kc.z), ang);
    newUP = Point3D::normalize(newUP);
    newRIGHT = rotateQuat( Point3D(this->ic.x, this->ic.y, this->ic.z) , Point3D(this->kc.x, this->kc.y, this->kc.z), ang);
    newRIGHT = Point3D::normalize(newRIGHT);

    this->ic = Vec3(newRIGHT.x, newRIGHT.y, newRIGHT.z);
    this->jc = Vec3(newUP.x, newUP.y, newUP.z);
}

/*void Camera::setImgSize(int width, int height) {
    if (width == imgWidth && height == imgHeight)
        return;
    if(img)
        delete img;

    imgWidth = width;
    imgHeight = height;
    
    img = new Image(imgWidth, imgHeight);
}*/
