#ifndef SCENE_H
#define SCENE_H

#include<vector>
#include "Primitive.h"
#include "Sphere.h"
#include "Object.h"
#include "triangle.h"
#include "importer.h"

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

    void addObject(const char* file_path, Material mat, glm::mat4 trans);

    void addTriangle(const Point& p1, const Point& p2, const Point& p3, Material mat);


    std::vector<Primitive*> objects;

    std::vector<Light> lights;

    int id;
};





#endif // SCENE_H
