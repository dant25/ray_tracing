#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"

class Material {
    public:
        Material();
        Material(const Color& amb, const Color& diff, const Color& spec, int k);

        void setAmbient(const Color& amb);
        void setDiffuse(const Color& diff);
        void setSpecular(const Color& spec);

        void setAll(const Color& c);

        Color getAmbient() const;
        Color getDiffuse() const;
        Color getSpecular() const;
        //private:
        Color ambient;
        Color diffuse;
        Color specular;
        int k;
};

#endif
