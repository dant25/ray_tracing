#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"

class Material {
    public:
        Material();
        Material(const Color& amb, const Color& diff, const Color& spec, int k);

        Material(const Color& amb, const Color& diff, const Color& spec, int k, float ka, float kd, float ks);

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

        float Ka, Kd, Ks;

        int k;
};

#endif
