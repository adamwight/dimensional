/* This is a very simple program to create the class of koch curves */

#include <math.h>
#include <stdio.h>

const int debug = 1;

#include "drawer.h"
#include "generator.h"

// rethink this, into fractal index into stored list of endpoints.
// address is ({iter?}, j
// only new points stored (for koch, 40% savings)
// operation[](fractal_index i)
//
//class segments
//{
//    static const segments CLASSIC_KOCH(4,
//        { { 0, 0 },
//          { 1/3.0, 0 },
//          { 1/3.0 + cos(pi/3) / 3, sin(pi/3) / 3) },
//          { 2/3.0, 0 },
//          { 1, 0 } });
//
//    cartesian* points;
//
//    public segments(int n_segments, double[][2] points)
//    {
//        this->points = points;
//    }
//};
//
//class segments_linked : public segments
//{
//    segments_linked* prev;
//    segments_linked* next;
//public:
//    segments_linked operator+=(segments_linked& next) {
//        next->next = this->next;
//        this->next = next;
//    }
//};
//

class koch_curve
{
public:
    const int& segments;
    generator g;

    double min_length;

    drawer* draw;

    koch_curve(drawer* draw, const generator& gen, double rez) :
        draw(draw),
        g(gen), segments(gen.segments),
        min_length(rez)
    {
    }

    cartesian cog;
    long ncog, nseg;

    void generate()
    {
      if (debug > 1)
          printf("generating\n");
      //cog = cartesian(0,0);
      //ncog = 0;
      nseg = 0;

      g.create_deltas();
      draw_generator();
      draw->move_to(g[0]);
      generate(1, g[0], g[segments]);
      draw_generator();

      //cog *= 1/(double)ncog;
      //draw->handle(cog);
      if (debug > 1) {
          printf("%ld segments rendered\n", nseg);
      //    if (debug > 1)
      //        printf("%ld CoG segments, at %f %f\n", ncog, cog.x, cog.y);
      }
    }

    void generate(int iters, cartesian origin, cartesian endpoint)
    {
      //if (debug > 1)
          ////printf("generation %d: start (%f, %f) f(): th %f scale %f\n", iters, origin.x, origin.y, xform.th, xform.r);
      //if (iters > MAX_ITERS && nsegs > MAX_SEGS)
        //return;
        //log once

      polar orientation = endpoint - origin;
      cartesian* points = new cartesian[segments + 1];
      points[0] = origin;
      for (int i = 0; i < segments; i++) {
        points[i + 1] = points[i] + orientation * g.d[i];
      }

      //if (iters == 5) {
      //    cog += points[0] + xform / 2;
      //    ncog ++;
      //}
      if (orientation.r < min_length) {
          draw->line_to(points[segments]);
          //draw->line(points[0], points[segments]);
          nseg++;
      } else {
          for (int i = 0; i < segments; i++) {
          //for (int i = 0; i <= segments; i++) {
            //int next = (i + 1) % (segments + 1); // could be a loop
            int next = (i + 1);
            if (g[i].link == LINE)
                generate(iters + 1, points[i], points[next]);
            else
                draw->move_to(points[next]);
          }
      }

      delete(points);
    }


    void draw_generator()
    {
        draw->fg(color::red);

        for (int i = 0; i < segments; i++) {
            if (g[i].link == LINE)
                draw->line(g[i], g[i + 1]);
            draw->handle(g[i]);
        }
        draw->handle(g[segments]);

        draw->fg(color::black);
    }

    void refresh() {
        draw->clear();
        generate();
    }
};
