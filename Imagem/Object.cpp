#include "Object.h"
#include <cmath>
#include <iostream>


Object::Object()
{

}

bool Object::Intersect(const Ray& ray, std::list<Interval> &interval)
{
    std::list<Interval> inter;
    if( box.Intersect(ray, inter) == false )
        return false;

    bool flag = false;
    float menor_dist = HUGE_VALF;

    Interval temp;
//std::cout << triangles.size() << std::endl;
    #pragma omp parallel for
    for(int i=0; i<triangles.size(); i++)
    {
        if( triangles[i]->Intersect(ray, interval) )
        {
            #pragma omp critical
            {
                if( interval.back().begin.dist < menor_dist )
                {
                    menor_dist = interval.back().begin.dist;
                    temp = interval.back();
                }

                flag = true;
            }
        }
    }

    interval.push_front(temp);

    return flag;
}
