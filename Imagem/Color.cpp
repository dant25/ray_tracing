#include "Color.h"

Color::Color(float red, float green, float blue) : r(red), g(green), b(blue) {

}

Color Color::operator*(const Color& c) const {
    return Color(r*c.r, g*c.g, b*c.b);
}

Color& Color::operator*=(const Color& c) {
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

void Color::setColor(float red, float green, float blue) {
    r = red;
    g = green;
    b = blue;

    if (r > 1)
        r = 1;
    if (g > 1)
        g = 1;
    if (b  > 1)
        b = 1;
}

bool operator==(const Color& c1 , const Color& c2) {
    if(c1.r == c2.r && c1.g == c2.g && c1.b == c2.b)
        return true;
    return false;
}
