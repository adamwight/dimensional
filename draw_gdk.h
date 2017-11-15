#ifndef __GDK_DRAW__
#define __GDK_DRAW__

#include "drawer.h"

class draw_gdk : public drawer
{
    GdkDrawable* window;
    GdkGC* gc;
    bool in_path;
    screen cursor;
    
public:
    draw_gdk(GdkDrawable* window, GdkGC* gc) {
        this->window = window;
        this->gc = gc;
        in_path = false;
    }

    void line_to(cartesian p2) {
      screen s2 = p2;
      if (!in_path)
          cursor = s2;
      if (debug > 1)
          printf("line_to: (%d, %d) (%d, %d)\n", cursor.x, cursor.y, p2.x, p2.y);
      gdk_draw_line(window, gc, cursor.x, cursor.y, s2.x, s2.y);
      cursor = s2;
      in_path = true;
    }

    void move_to(cartesian p2) {
      screen s2 = p2;
      cursor = s2;
    }

    void line(cartesian p1, cartesian p2) {
      if (debug > 1)
          printf("line: (%f, %f) to (%f, %f)\n", p1.x, p1.y, p2.x, p2.y);

      screen s1 = p1;
      screen s2 = p2;
      gdk_draw_line(window, gc, s1.x, s1.y, s2.x, s2.y);
      cursor = s2;
      in_path = true;
    }

    void handle(cartesian p) {
      const int r = 4;
      screen s(p);
      gdk_draw_arc(window, gc, FALSE, s.x - r, s.y - r, r*2, r*2, 0, 360*64);
    }

    void fg(color c) {
        if (debug > 2)
            printf("fg: c(%f %f %f)\n", c.r, c.g, c.b);
        rgb_fg((int)(c.r * 65535.0), (int)(c.g * 65535.0), (int)(c.b * 65535.0));
    }

    void rgb_fg(int r, int g, int b) {
        if (debug > 1)
            printf("fg: (%d %d %d)\n", r, g, b);
        GdkColor c;
        c.red = r; c.green = g; c.blue = b;
        gdk_gc_set_rgb_fg_color(gc, &c);
    }

    void clear() {
        fg(color::white);
        gdk_draw_rectangle(window, gc, TRUE, 0, 0, 700, 700);
    }
};

#endif
