#include "Material.h"

Material::Material() {
}

Material::Material(const Color& amb, const Color& diff, const Color & spec, int k) : ambient(amb), diffuse(diff), specular(spec), k(k){

}

void Material::setAmbient(const Color& amb) {
    ambient = amb;
}

void Material::setDiffuse(const Color& diff) {
    diffuse = diff;
}

void Material::setSpecular(const Color& spec) {
    specular = spec;
}

void Material::setAll(const Color& c){
    setAmbient(c);
    setDiffuse(c);
    setSpecular(c);
}

Color Material::getAmbient() const{
    return ambient;
}

Color Material::getDiffuse() const{
    return diffuse;
}

Color Material::getSpecular() const{
    return specular;
}

