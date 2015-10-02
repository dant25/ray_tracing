#include<iostream>
#include "Camera.h"
#include "Ray.h"
#include "Primitive.h"


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
    float xw = 2*pos.z*(wImg - imgWidth/2.0) / imgWidth;
    float yw = 2*pos.z*(hImg - imgHeight/2.0) / imgHeight;

    //std::cout << wImg << " " << xw << "\t"<< hImg << " " << yw << std::endl;

    //ponto projetado
    Point p = pos + kc*(-pos.z) + ic*xw + jc*yw;
    Vec3 dir = p - pos;
    dir.normalize();
    //Inicializa o raio
    return Ray(pos, dir);
}

Point Camera::getPos() {
    return pos;
}

void Camera::setPos(Point pos) {
    this->pos = pos;
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

/*void Camera::setImgSize(int width, int height) {
    if (width == imgWidth && height == imgHeight)
        return;
    if(img)
        delete img;

    imgWidth = width;
    imgHeight = height;
    
    img = new Image(imgWidth, imgHeight);
}*/
