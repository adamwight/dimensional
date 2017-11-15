#ifndef __COORDS_H__
#define __COORDS_H__

const double pi = 3.14159265;

class cartesian
{
public:
    double x, y;

    cartesian() { x = 0; y = 0; }
    cartesian(const cartesian& src) { x = src.x; y = src.y; }
    cartesian(const class polar& src);
    cartesian(double x, double y) {
         this->x = x; this->y = y;
    }
    cartesian operator+(const cartesian& b) {
        return cartesian(x + b.x, y + b.y);
    }

    cartesian operator-(const cartesian& b) {
        return cartesian(x - b.x, y - b.y);
    }

    cartesian operator+(const polar& b) { return *this + cartesian(b); }

    cartesian& operator+=(const cartesian& b) { return *this = *this + b; }

    cartesian& operator*=(double s) { x *= s; y *= s; return *this; }
};

class polar
{
public:
    double r, th;

    polar() { r = 0; th = 0; }

    polar(const cartesian& src) {
        r = sqrt(src.x * src.x + src.y * src.y);
        th = atan2(src.y, src.x);
    }

    polar operator*(const polar& b) {
        polar result(*this);
        result.r *= b.r;
        result.th += b.th;

        //printf("applied xform: %fr%f -> %fr%f\n", src.r, src.th, result.r, result.th);
        return result;
    }

    polar operator/(double scale) {
        polar result(*this);
        result.r /= scale;
        return result;
    }
};

int offX = 200;
int offY = 200;
double screenScale = 350;

class screen
{
public:
    int x, y;

    screen() { x = 0; y = 0; }

    screen(const cartesian& src) {
        x = offX + int(screenScale * src.x);
        y = offY + int(screenScale * src.y);
    }

    static cartesian unmap(int x, int y) {
        return cartesian((x - offX) / screenScale, (y - offY) / screenScale);
    }
};

cartesian::cartesian(const polar& src)
{
    x = src.r * cos(src.th);
    y = src.r * sin(src.th);
}

#endif
