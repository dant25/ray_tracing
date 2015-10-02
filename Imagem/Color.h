#ifndef _COLOR_H_
#define _COLOR_H_

class Color {
    friend bool operator==(const Color& c1, const Color &c2);
    public:
        Color(float red = 0, float green = 0, float blue = 0);

        Color operator*(const Color& c) const;
        Color& operator*=(const Color& c);
        void setColor(float red, float green, float blue);
        //private:
        float r, g, b;
};

bool operator==(const Color &c1, const Color& c2);
#endif
