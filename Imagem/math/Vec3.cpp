#include "Vec3.h"
#include "Point.h"

Vec3::Vec3() {

}
Vec3::Vec3(const Point& p) {
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
}
Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

}

Vec3 Vec3::operator+(const Vec3& v) const {
	return Vec3(x + v.x, y + v.y, z + v.z );
}

Vec3& Vec3::operator+=(const Vec3& v){
	x+= v.x; y += v.y; z+= v.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z );
}

Vec3& Vec3::operator-=(const Vec3& v){
    x-= v.x; y -= v.y; z-= v.z;
    return *this;
}

//

Vec3 Vec3::operator*(float f) const{
	return Vec3(x*f, y*f, z*f);
}

Vec3& Vec3::operator*=(float f) {
	x *= f; y*= f; z*= f;
	return *this;
}

Vec3 Vec3::operator/(float f) const{
	if (fabs(f) < 0.0004) {
		//TODO tratar esse erro
		return *this;
	}
	return Vec3(x/f, y/f, z/f);
}

Vec3& Vec3::operator/=(float f) {
	if (fabs(f) < 0.0004) {
		//TODO tratar isso
		return *this;
	}

	x/=f; y/=f; z/=f;
	return *this;
}

float Vec3::length() {
	return sqrt(x*x + y*y + z*z );
}

void Vec3::normalize() {
	float len = this->length();
	x/=len; y /= len; z /= len;
}

float Dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Cross(const Vec3& v1, const Vec3& v2) {
    return Vec3((v1.y*v2.z) - (v1.z*v2.y),
                      (v1.z*v2.x) - (v1.x*v2.z),
                      (v1.x*v2.y) - (v1.y*v2.x));
}
