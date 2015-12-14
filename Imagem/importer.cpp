#include "importer.h"

ObjImporter::ObjImporter()
{
    max = maxX = maxY = maxZ = -HUGE_VALF ;

    min = minX = minY = minZ = HUGE_VALF ;
}

ObjImporter::ObjImporter(const char* file_path, bool haveTexture)
{
    max = maxX = maxY = maxZ = -HUGE_VALF ;

    min = minX = minY = minZ = HUGE_VALF ;

    this->haveTexture=haveTexture;

    loadData(file_path);
}

void ObjImporter::findMinMax()
{
    if( minX < minY )
    {
        if( minX < minZ )   min = minX;
        else min = minZ;
    }
    else
    {
        if( minY < minZ )   min = minY;
        else min = minZ;
    }
///=============
    if( maxX > maxY )
    {
        if( maxX > maxZ )   max = maxX;
        else max = maxZ;
    }
    else
    {
        if( maxY > maxZ )   max = maxY;
        else max = maxZ;
    }
}

void ObjImporter::loadData(const char* &file_path)
{
    std::ifstream file( file_path, std::ifstream::in);
    if( !file ){
        std::cout << "Erro ao abrir o arquivo: " << file_path << std::endl;
    }
    string tipo;

    while (file.good())
    {
        tipo = "_";
        file >> tipo;
        if (tipo == "v" ) {        ///VERTICES
            float v1, v2, v3;

            file >> v1;
            file >> v2;
            file >> v3;
//            float x = atof (v1.c_str());
//            float y = atof (v2.c_str());
//            float z = atof (v3.c_str());
//            x = (double)strtod( v1.c_str(), NULL );
//            y = (double)strtod( v2.c_str(), NULL );
//            z = (double)strtod( v3.c_str(), NULL );
//            cout << "PONTOS " << x << " " << y << " " << z << endl;
//            cout << "PONTOS == " << v1 << " " << v2 << " " << v3 << endl;



            if( v1 > maxX )  maxX = v1;
            if( v2 > maxY )  maxY = v2;
            if( v3 > maxZ )  maxZ = v3;

            if( v1 < minX )  minX = v1;
            if( v2 < minY )  minY = v2;
            if( v3 < minZ )  minZ = v3;

            vertices.push_back(new Ponto(v1, v2, v3));

            continue;
        }

        if (tipo == "vt" ) {        ///TEXTURAS
            float u, v;

            file >> u;
            file >> v;

            //cout << "Textura == " << u << " " << v << endl;

            texturas.push_back(Textura(u, v, 0));

            continue;
        }

        if (tipo == "vn" ) {
            float x, y, z;

            file >> x;
            file >> y;
            file >> z;
//            double x = atof (v1.c_str());
//            double y = atof (v2.c_str());
//            double z = atof (v3.c_str());
            //cout << "NORMAIS " << x << " " << y << " " << z << endl;

            normals.push_back( Ponto(x, y, z) );

            continue;
        }

        if (tipo == "f" ) {
            Face face;
            Ponto normal;

            if( !this->haveTexture )
            {
                file >> face.x; file >> normal.x;
                file >> face.y; file >> normal.y;
                file >> face.z; file >> normal.z;
            }
            else
            {
                Ponto textura;
                file >> face.x; file >> textura.x; file >> normal.x;
                file >> face.y; file >> textura.y; file >> normal.y;
                file >> face.z; file >> textura.z; file >> normal.z;

                textura.x--;
                textura.y--;
                textura.z--;

                textura_faces.push_back(textura);
            }

            face.x--;
            face.y--;
            face.z--;
            normal.x--;
            normal.y--;
            normal.z--;

            //cout << "FACES = " << face.x << " " << face.y << " " << face.z << endl;
            //cout << "NORMS = " << normal.x << " " << normal.y << " " << normal.z << endl;

            faces.push_back(face);
            normals_faces.push_back(normal);

            continue;
        }
    }

    file.close();

    findMinMax();
}
