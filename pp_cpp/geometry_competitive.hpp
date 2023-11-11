#ifndef OH_BOI_THIS_BITCH_IS_GETTING_COMPETITIVE
#define OH_BOI_THIS_BITCH_IS_GETTING_COMPETITIVE
// https://cp-algorithms.com/geometry/circle-line-intersection.html
/* sto copiando codice a cazzo da internet finchè non si trova
 * una funzione con la grazia di funzionare
 */

#include "geometry.hpp"
#include <vector>
#include <cmath>

#define EPS 0.000000001

struct LineaCompetitiva {
    // linea come insieme dei punti in cui ax + by + c = 0;
    double a;
    double b;
    double c;
    LineaCompetitiva(double a, double b, double c):a(a),b(b),c(c) {}

    // y = mx + q
    // mx -y + q = 0
    static LineaCompetitiva from_mx_q(double m, double q) {
        return LineaCompetitiva(m, -1, q);
    }

    static LineaCompetitiva from_linea(const Line l) {
        return LineaCompetitiva::from_mx_q(l.m, l.q);
    }
};


std::vector<Point> soluzioni_competitive(const LineaCompetitiva lc, double r) {
    // si assume circonferenza centrata in 0 0
    // lc va traslata prima di passarla a questa funzione

    std::vector<Point> soluzioni;
    double a = lc.a;
    double b = lc.b;
    double c = lc.c;

    double x0 = -a*c/(a*a+b*b), y0 = -b*c/(a*a+b*b);
    if (c*c > r*r*(a*a+b*b)+EPS)
        return soluzioni;
    else if (fabs (c*c - r*r*(a*a+b*b)) < EPS) {
        soluzioni.push_back(Point(x0, y0));
        return soluzioni;
    }
    else {
        double d = r*r - c*c/(a*a+b*b);
        double mult = sqrt (d / (a*a+b*b));
        double ax, ay, bx, by;
        ax = x0 + b * mult;
        bx = x0 - b * mult;
        ay = y0 - a * mult;
        by = y0 + a * mult;
        soluzioni.push_back(Point(ax, ay));
        soluzioni.push_back(Point(bx, by));
        return soluzioni;
    }
}

std::vector<Point> competitive_line_intersects(const Line l,
                                               const Point c,
                                               double r) {
    LineaCompetitiva lc = LineaCompetitiva::from_linea(l.trasla(c.x, c.y));
    std::vector<Point> shifted = soluzioni_competitive(lc, r);
    std::vector<Point> res = std::vector<Point>();
    for (Point s : shifted) {
        res.push_back(Point(s.x + c.x, s.y + c.y));
    }
    return res;
}

std::vector<Point> competitive_segment_intersects(const Segment s,
                                                  const Point c,
                                                  double r) {
    std::vector<Point> line = competitive_line_intersects(Line::from_points(s.start, s.end), c, r);
    std::vector<Point> res = std::vector<Point>();
    for (Point l : line) {
        if(s.point_in_rect(l))
            res.push_back(l);
    }
    return res;
}

#undef EPS

#endif
