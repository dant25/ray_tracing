#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include<vector>
#include "Primitive.h"
#include "Sphere.h"


struct Light {
    Color color;
    Point position;
    float attenuation;
};

class Scene
{
public:
    Scene();

    bool Intersect(const Ray& ray, Intersection &intersect);

    void addLight(const Point& center, int r, int g, int b, float att);

    void addSphere(const Point& center, float radius, Material mat);




    std::vector<Primitive*> objects;

    std::vector<Light> lights;


    int numSphere;
    int id;
};





#endif // SCENE_H
