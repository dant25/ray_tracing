#include "scene.h"


//int Scene::numSphere = 0;

Scene::Scene()
{
    this->numSphere = 0;
    this->id = 0;
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
void Scene::addLight(const Point& center, int r, int g, int b, float att)
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
void Scene::addSphere(const Point& center, float radius, Material mat) {
    this->numSphere++;
    Sphere *s = new Sphere(center, radius);
    s->material=mat;
    s->id = this->id;
    this->id++;

    objects.push_back(s);
}

void Scene::addTriangle(const Point& p1, const Point& p2, const Point& p3, Material mat)
{
    Triangle *t = new Triangle(p1, p2, p3, mat);
    t->id = this->id;
    this->id++;

    objects.push_back(t);
}

void Scene::addTorus(const char* file_path, Material mat)
{
    ObjImporter importer( file_path, false);

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
        t->id = this->id;
        this->id++;

        objects.push_back(t);
    }
}
