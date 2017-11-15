#ifndef __GENERATOR_H__
#define __GENERATOR_H__

//#include <string.h>

#include "coords.h"

typedef enum linkage { LINE, GAP, END };

class segment : public cartesian
{
public:
    // how this point is attached to the next
    linkage link;

    segment() { }
    segment(const segment& src, linkage link = LINE)
        : cartesian(src), link(src.link) { }
    segment(double x, double y, linkage link = LINE)
        : cartesian(x, y), link(link) { }
};

class generator
{
    segment* points;

public:
    polar* d;
    cartesian sum;
    int segments;

    generator(const generator& gen)
            : d(NULL)
            //: generator(gen.segments) {
    {
        segments = gen.segments;
        points = new segment[segments + 1];
        //memcpy(points, gen.points, sizeof(double) * segments);
        for (int i = 0; i < segments + 1; i++)
            points[i] = gen.points[i];
    }

    generator(int nseg) : d(NULL) {
        segments = nseg;
        points = new segment[segments + 1];
    }

    ~generator() {
        if (d)
            delete d;
    }

    segment& operator[](int i) { return points[i]; }
    segment& operator[](int i) const { return points[i]; }

    void insert(int where, const segment& another) {
        segments++;
        segment* old_points = points;
        points = new segment[segments + 1];
        for (int i = 0; i < where; i++)
            points[i] = old_points[i];
        for (int i = segments - 1; i >= where; i--)
            points[i + 1] = old_points[i];
        points[where] = another;
        delete old_points;
    }

    const int find_handle(int x, int y) const {
        int winner = -1;
        int winwith = 100;

        for (int i = 0; i < segments + 1; i++) {
            screen s(points[i]);
            int d = (int)sqrt((s.x - x) * (s.x - x) + (s.y - y) * (s.y - y));
            if (d < 10 && d < winwith) {
                winner = i;
                winwith = d;
            }
        }
        return winner;
    }

    void create_deltas()
    {
        if (d)
            delete d;
        d = new polar[segments];
        polar normalize = points[segments] - points[0];
        //XXX
        normalize.r = 1 / normalize.r;
        normalize.th = -normalize.th;

        for (int i = 0; i < segments; i++) {
            d[i] = polar(points[i + 1] - points[i]) * normalize;

            if (debug > 1)
                printf("v%d = %fr%f\n", i, d[i].r, d[i].th);
        }

        sum = points[segments] - points[0];
    }
};

#endif
