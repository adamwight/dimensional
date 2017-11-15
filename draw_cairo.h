#ifndef __CAIRO_DRAW__
#define __CAIRO_DRAW__

#include <cairo/cairo-ps.h>

#include "drawer.h"

class draw_cairo : public drawer
{
    cairo_t* cr;
    cairo_surface_t* surface;
    bool in_path;
    
public:
    // single-render
    draw_cairo(const char* filename) {
        surface = cairo_ps_surface_create(filename, 5 * 72, 5 * 72);
        cr = cairo_create(surface);
        cairo_set_line_width(cr, .05);
        //cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
        in_path = false;
    }

    cartesian xform(cartesian p) { return cartesian(p.x * 500 + 50, p.y * 500 - 150); }

    void line_to(cartesian p2) {
        cartesian s2 = xform(p2);
        if (!in_path) {
          //cairo_new_path?
          cairo_new_path(cr);
          cairo_move_to(cr, s2.x, s2.y);
        }
        cairo_line_to(cr, s2.x, s2.y);
        in_path = true;
    }

    void move_to(cartesian p2) {
        cartesian s2 = xform(p2);
        cairo_move_to(cr, s2.x, s2.y);
    }

    void line(cartesian p1, cartesian p2) {
        if (debug > 1)
            printf("line: (%f, %f) to (%f, %f)\n", p1.x, p1.y, p2.x, p2.y);

        cartesian s1 = xform(p1);
        cartesian s2 = xform(p2);
        cairo_move_to(cr, s1.x, s1.y);
        cairo_line_to(cr, s2.x, s2.y);
        in_path = true;
    }

    void handle(cartesian p) {
        const double r = .1;
        screen s(p);
        cairo_move_to(cr, s.x, s.y);
        cairo_arc(cr, s.x, s.y, r, 0, 3.141 * 2);
    }


    void fg(color c)
    {
        cairo_set_source_rgb(cr, c.r, c.g, c.b);
    }

    void rgb_fg(int r, int g, int b) {
        color c(r, g, b);
        fg(c);
    }

    void clear() {
        fg(color::white);
        //cairo_rectangle(cr, 0, 0, 700, 700);
    }

    void blit() {
        cairo_stroke(cr);
        cairo_surface_finish(surface);
        cairo_destroy(cr);
    }
};

#endif
