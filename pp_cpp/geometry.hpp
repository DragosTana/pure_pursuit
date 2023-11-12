#ifndef PP_CPP_GEOMETRY_H
#define PP_CPP_GEOMETRY_H

#include<vector>
#include<cmath>
#include<iostream>
#include<limits>

#define DEBUGGING

int sign(int n) {
  if (n) {
      if(n > 0)
        return 1;
      else
          return -1;
  }
  return 0;
}

class Point {
public:
    double x;
    double y;

    Point(double x, double y):x(x),
                            y(y) {}

    double distance(const Point& other) const {
        return sqrt(distance_square(other));
    }

    double distance_square(const Point& other) const {
        return pow((x - other.x), 2) + pow((y - other.y), 2);
    }

    bool is_closer(const Point& closer, const Point& farther) const {
        return distance_square(closer) <= distance_square(farther);
    }

    Point closest(const Point& p1, const Point& p2) const {
        if(is_closer(p1, p2))
            return p1;
        return p2;
    }

    /* if this, closer, and farther, are on the same segment, we can avoid
     * having to compute the distances, checking on only one coordinate should
     * be enough
     */
    bool is_closer_same_segment(const Point& closer, const Point& farther) const {
        if(closer.x == farther.x) {
            return fabs(y-closer.y) <= fabs(y-farther.y);
        }
        return fabs(x-closer.x) <= fabs(x-farther.x);
    }


    Point closest_same_segment(const Point& p1, const Point& p2) const {
        if(is_closer_same_segment(p1, p2))
            return p1;
        return p2;
    }
};

class Circle {
  public:
    Point center;
    double radius;
    Circle(Point center, double radius):center{center},radius{radius}{}
};
    

class Line {
public:
    double m;
    double q;

    static constexpr double very_big =   10000000;
    static constexpr double very_small = 0.0000001;

    Line(double m, double q):m(m),q(q){}

    static Line from_points(const Point& a, const Point& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;

        double m = (fabs(dx) >= very_small) ? dy / dx : very_big;
        double q = a.y - m*a.x;
        return Line(m,q);
    }

    Line traslate(double x0, double y0) const {
        /* dati this, x0, e y0, ritorna la retta r tale che
         * \[\forall (x,y) \in this, (x+x0, y+y0) \in r\]
         *
         * sapendo che (0,q) e (1, m+q) sono in this
         * sappiamo che (x0, q+y0) e (1+x0, m+q+y0) sono in r
         * bastano due punti quindi
         */

        return Line::from_points(Point(x0, q + y0),
                                 Point(1+x0, m+q+y0));
    }

    std::vector<Point> centered_circle_intersects(const double r) const {
        /* https://cp-algorithms.com/geometry/circle-line-intersection.html
         *
         * la funzione assume una circonferenza di raggio r centrata nell'origine
         * se la circonferenza iniziale era centrata in xc,yc, allora l va traslata
         * di (-xc, -yc) prima di essere passata alla funzione, e i punti risultanti
         * vanno ritraslati di (xc, yc) una volta ottenuti
         */

        // a, b, e c tali l sia il luogo dei punti dove ax + by + c = 0
        double a = m;
        double b = -1;
        double c = q;
        const double epsilon = 0.000000001;

        double x0 = -a*c/(a*a+b*b), y0 = -b*c/(a*a+b*b);
        if (c*c > r*r*(a*a+b*b)+epsilon)
            return {};
        else if (fabs (c*c - r*r*(a*a+b*b)) < epsilon) {
            return {Point(x0, y0)};
        }
        else {
            double d = r*r - c*c/(a*a+b*b);
            double mult = sqrt (d / (a*a+b*b));
            double ax, ay, bx, by;
            ax = x0 + b * mult;
            bx = x0 - b * mult;
            ay = y0 - a * mult;
            by = y0 + a * mult;
            return {Point(ax, ay), Point(bx, by)};
        }
    }

    std::vector<Point> circle_intersects(const Circle& cr) const {
        const Point c = cr.center;
        const double r = cr.radius;
        const Line cen_line = traslate(-c.x, -c.y);
        const std::vector<Point> cen_points = cen_line.centered_circle_intersects(r);
        std::vector<Point> res = std::vector<Point>();
        for (Point s : cen_points) {
            res.push_back(Point(s.x + c.x, s.y + c.y));
        }
        return res;
    }
};


class Segment {
public:
    Point start;
    Point end;

    Segment(const Point& start, const Point& end)
        : start(start.x, start.y), end(end.x, end.y) {}

    /*
     * Returns true if the point is valid, false otherwise.
     * Valid means that the point is inside the rectangle defined by the segment.
     * @param[in] p the point to check
     */
    const bool point_in_rect(const Point& p) const {
        return
            (p.x >= std::min(start.x, end.x)) &&
            (p.x <= std::max(start.x, end.x)) &&
            (p.y >= std::min(start.y, end.y)) &&
            (p.y <= std::max(start.y, end.y));
    }

    Line extend_to_line() const {
        return Line::from_points(start, end);
    }

    std::vector<Point> circle_intersects(const Circle cr) const {
        const std::vector<Point> line = extend_to_line().circle_intersects(cr);
        std::vector<Point> res = std::vector<Point>();
        for (Point l : line) {
            if(point_in_rect(l))
                res.push_back(l);
        }
        return res;
    }
};

#endif
