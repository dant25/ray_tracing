#ifndef CAMERA_H_
#define CAMERA_H_

#include "math/Vec3.h"
#include "math/Point.h"
#include "Ray.h"

class Scene;

class Camera {
public:
    Camera();
    Camera(int width, int height);
    ~Camera();

    Ray createRay(int wImg, int hImg);
    void render();

    Vec3 getLookAt();
    void lookAt( Point point );

    void Rotation_i(float ang);
    void Rotation_j(float ang);
    void Rotation_k(float ang);

    //void setImgSize(int width, int height);

    Point getPos();
    void setPos(Point pos);

    int imgWidth;
    int imgHeight;

    Vec3 ic;
    Vec3 jc;
    Vec3 kc;
    Point pos;
};

#endif


