#ifndef __DRAWER_H__
#define __DRAWER_H__

#include "coords.h"
#include "color.h"

class drawer
{
public:
    virtual void line(cartesian p1, cartesian p2) {};
    virtual void line_to(cartesian p2) {};
    virtual void move_to(cartesian p2) {};

    virtual void handle(cartesian p) {};

    virtual void fg(color c) {};
    virtual void rgb_fg(int r, int g, int b) {};

    virtual void clear() {};
};

#endif
