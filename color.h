#ifndef __COLOR_H__
#define __COLOR_H__

class color
{
public:
    // this impl is [0-1]
    double r, g, b;

    color(double red, double green, double blue) {
        r = red; g = green; b = blue;
    }
    color(int red, int green, int blue) {
        r = red / 65535.0; g = green / 65535.0; b = blue / 65535.0;
    }

    static const color red, black, white;

    // arg [0-1]
    //static const color& ramp(double i) { return color(i, i / 2, i / 2); }
};

const color color::red(1.0, 0.0, 0.0);
const color color::black(0.0, 0.0, 0.0);
const color color::white(1.0, 1.0, 1.0);

#endif
