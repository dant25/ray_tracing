#ifndef IMPORTER_H
#define IMPORTER_H

#include <fstream>
#include <iostream>

//#include <stdlib.h>
#include <cstdlib>
#include <cstring>
//#include <glm/glm.hpp>
#include "math/Point.h"

#include <vector>

#define Ponto Point
#define Face Point
#define Textura Point

using namespace std;

class ObjImporter
{
    public:
        ObjImporter();
        ObjImporter(const char* file_path, bool haveTexture);
        ~ObjImporter() {};

        void loadData(const char* &file_path);

        void findMinMax();

        std::vector<Ponto*> vertices;
        std::vector<Textura> texturas;
        std::vector<Ponto> normals;
        std::vector<Face> faces;

        std::vector<Ponto> normals_faces;
        std::vector<Ponto> textura_faces;
        bool haveTexture;

        float maxX, minX;
        float maxY, minY;
        float maxZ, minZ;
        float max, min;

};

#endif // IMPORTER_H
