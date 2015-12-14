#include "scene.h"
#include <iostream>

//int Scene::numSphere = 0;

Scene::Scene()
{
    this->id = 1;
}


/**
 * Função para verificar a interseção de um raio com toda a cena.
 * Retorna true se houver alguma interseção, false caso contrário.
 **/
bool Scene::Intersect(const Ray& ray, Intersection &intersect) {
    std::vector<Primitive*>::iterator iter;
    std::list<Intersection> intersectionPoints;

    //std::list<Interval> intervalList;

    for(iter = objects.begin(); iter != objects.end(); iter++) {

        if(ray.IDfonte == (*iter)->id)      ///NÃO TESTA COLISÃO COM O PROPRIO OBJETO
                continue;

        std::list<Interval> intervalList;

        if( (*iter)->Intersect(ray, intervalList) )
        {
            //Assumindo que o primeiro elemento da lista de
            //intervalos é o mais próximo da camera.
            Intersection intersectPt = intervalList.front().begin;

            intersectPt.dist = (intervalList.front().begin.point - ray.o).length();

            //intersectPt.objIndex = distance(objects.begin(), iter);
            //intersectPt.objIndex = intervalList.front().begin.objIndex;
            //intersectPt.distintervalList.front().begin.
            intersectionPoints.push_back(intersectPt);
        }
    }

    if (intersectionPoints.size() == 0)
        return false;


    std::list<Intersection>::iterator it = intersectionPoints.begin();

    Intersection closest = *it;

    it++;

    while (it != intersectionPoints.end()) {
        if (closest.dist > (*it).dist)
            closest = *it;

        it++;
    }
    intersect = closest;

    return true;
}

/**
 * Adicionar uma fonte luminosa
 **/
void Scene::addLight(const Point& center, float r, float g, float b, float att)
{
    Light luz;
    luz.color.setColor(r, g, b);
    luz.position = center;
    luz.attenuation = att;

    lights.push_back(luz);
}

/**
 * Adicionar uma primitiva do tipo esfera à cena
 **/
void Scene::addSphere(const Point& center, float radius, Material mat)
{
    Sphere *s = new Sphere(center, radius);
    s->material=mat;
    s->id = this->id;
    this->id++;

    objects.push_back(s);
}

/**
 * Adicionar uma primitiva do tipo triangulo à cena
 **/
void Scene::addTriangle(const Point& p1, const Point& p2, const Point& p3, Material mat)
{
    Triangle *t = new Triangle(p1, p2, p3, mat);
    t->id = this->id;
    this->id++;

    objects.push_back(t);
}

/**
 * Adicionar uma primitiva genérica por aquivo .OBJ como triangulos
 **/
void Scene::addObject(const char* file_path, Material mat, glm::mat4 trans)
{
    Object *object = new Object();
    object->id = this->id;
    this->id++;

    ObjImporter importer( file_path, false);

    glm::vec4 pMax( importer.maxX, importer.maxY, importer.maxZ, 1.0 );
    glm::vec4 pMin( importer.minX, importer.minY, importer.minZ, 1.0 );
    pMax = trans * pMax;
    pMin = trans * pMin;

    float raio = sqrt( pow(pMax.x - pMin.x, 2.0) +
                    pow(pMax.y - pMin.y, 2.0) +
                    pow(pMax.z - pMin.z, 2.0) ) / 2.0;

    object->box = Sphere( Point( (pMax.x + pMin.x) / 2.0,
                                (pMax.y + pMin.y) / 2.0,
                                (pMax.z + pMin.z) / 2.0 ), raio );

    ///APLICANDO TRANSFORMAÇÕES NOS VERTICES
    for( int i=0; i < importer.vertices.size(); i++ )
    {
        glm::vec4 newP( importer.vertices[i]->x, importer.vertices[i]->y, importer.vertices[i]->z, 1.0 );
        newP = trans * newP;
        importer.vertices[i]->x = newP.x;
        importer.vertices[i]->y = newP.y;
        importer.vertices[i]->z = newP.z;
    }

    ///APLICANDO TRANSFORMAÇÕES NAS NORMAIS
    for( int i=0; i < importer.normals.size(); i++ )
    {
//std::cout << "NORM " << i << ": " << importer.normals[i].x << " " << importer.normals[i].y << " " << importer.normals[i].z << " " << std::endl;
        glm::vec4 newP( importer.normals[i].x, importer.normals[i].y, importer.normals[i].z, 1.0 );
        newP = trans * newP;

        glm::vec4 ori( 0,0,0, 1.0 );
        ori = trans * ori;

        newP = newP - ori;

        importer.normals[i].x = newP.x;
        importer.normals[i].y = newP.y;
        importer.normals[i].z = newP.z;
        importer.normals[i].normalize();
//std::cout << "NORM SAIDA " << i << ": " << importer.normals[i].x << " " << importer.normals[i].y << " " << importer.normals[i].z << " " << std::endl;
    }

    Point p1, p2, p3;

    for( int i=0; i < importer.faces.size(); i++ )
    {
        p1 = Point( importer.vertices[ importer.faces[i].x ]->x,
                    importer.vertices[ importer.faces[i].x ]->y,
                    importer.vertices[ importer.faces[i].x ]->z );

        p2 = Point( importer.vertices[ importer.faces[i].y ]->x,
                    importer.vertices[ importer.faces[i].y ]->y,
                    importer.vertices[ importer.faces[i].y ]->z );

        p3 = Point( importer.vertices[ importer.faces[i].z ]->x,
                    importer.vertices[ importer.faces[i].z ]->y,
                    importer.vertices[ importer.faces[i].z ]->z );

        Triangle *t = new Triangle(p1, p2, p3, mat);

        t->n1 = Vec3(importer.normals[ importer.normals_faces[i].x ].x,
                importer.normals[ importer.normals_faces[i].x ].y,
                importer.normals[ importer.normals_faces[i].x ].z);
        t->n2 = Vec3(importer.normals[ importer.normals_faces[i].y ].x,
                importer.normals[ importer.normals_faces[i].y ].y,
                importer.normals[ importer.normals_faces[i].y ].z);
        t->n3 = Vec3(importer.normals[ importer.normals_faces[i].z ].x,
                importer.normals[ importer.normals_faces[i].z ].y,
                importer.normals[ importer.normals_faces[i].z ].z);

        /*t->vertexes_norm = Vec3( (importer.normals[ importer.normals_faces[i].x ].x
                                             + importer.normals[ importer.normals_faces[i].y ].x
                                             + importer.normals[ importer.normals_faces[i].z ].x) / 3.0,
                                 (importer.normals[ importer.normals_faces[i].x ].y
                                               + importer.normals[ importer.normals_faces[i].y ].y
                                               + importer.normals[ importer.normals_faces[i].z ].y) / 3.0,
                                 (importer.normals[ importer.normals_faces[i].x ].z
                                               + importer.normals[ importer.normals_faces[i].y ].z
                                               + importer.normals[ importer.normals_faces[i].z ].z) / 3.0
                                 );
        t->vertexes_norm.normalize();*/

        object->triangles.push_back(t);
    }

    objects.push_back(object);
}
