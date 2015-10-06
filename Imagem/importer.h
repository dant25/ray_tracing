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
        ObjImporter(const char* file_path, bool haveTexture) { this->haveTexture=haveTexture; loadData(file_path);};
        ObjImporter() {};
        ~ObjImporter() {};

        void loadData(const char* &file_path);

        std::vector<Ponto*> vertices;
        std::vector<Textura> texturas;
        std::vector<Ponto> normals;
        std::vector<Face> faces;

        std::vector<Ponto> normals_faces;
        std::vector<Ponto> textura_faces;
        bool haveTexture;


};

#endif // IMPORTER_H
