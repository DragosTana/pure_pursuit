#ifndef PP_CPP_GEOMETRY_H
#define PP_CPP_GEOMETRY_H

#include<vector>
#include<cmath>
#include<iostream>
#include<limits>

bool in_range(double a, double ex1, double ex2) {
    // memento, (a <= b <= c) NON funziona in c++
    return ((ex1 <= a) && (a <= ex2)) || ((ex2 <= a) && (a <= ex1));
}

std::vector<double>quadratic_solutions(double a, double b, double c) {
    double delta = b*b - 4*a*c;
    std::vector<double> res = std::vector<double>();
    if(delta >= 0) {
        res.reserve(2);
        double delta_root = sqrt(delta);
        res.push_back((-b + delta_root) / (2*a));
        res.push_back((-b - delta_root) / (2*a));
    }
    return res;
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

    bool is_closer_same_segment(const Point& closer, const Point& farther) const {
        if(closer.x == farther.x) {
            return fabs(y-closer.y) <= fabs(y-farther.y);
        }
        return fabs(x-closer.x) <= fabs(x-farther.x);
    }

    Point closest(const Point& p1, const Point& p2) const {
        if(is_closer(p1, p2))
            return p1;
        return p2;
    }

    Point closest_same_segment(const Point& p1, const Point& p2) const {
        if(is_closer_same_segment(p1, p2))
            return p1;
        return p2;
    }
};

#define VERY_SMALL (0.0000000001)
#define VERY_BIG   (10000000000)
class Line {
public:
    double m;
    double q;

    Line(double m, double q):m(m),q(q){}

    static Line from_points(const Point& a, const Point& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;

        double m = (fabs(dx) >= VERY_SMALL) ? dy / dx : VERY_BIG;
        double q = a.y - m*a.x;
        return Line(m,q);
    }

    std::vector<Point> circle_intersections(Point& c, double r) const {
        // https://math.stackexchange.com/questions/228841
        // how do i calculate the intersections of a straight line and a circle?
        double x0 = c.x;
        double y0 = c.y;
        std::vector<double> xs = quadratic_solutions
            ((m + 1),
             (2*(m*q - x0 - m*y0)),
             (x0*x0 + q*q - 2*q*y0 + y0*y0 - r*r));
        std::vector<Point> res = std::vector<Point>();
        res.reserve(xs.size());
        for(double x:xs) {
            res.push_back(Point(x, m*x + q)); 
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

    std::vector<Point> circle_intersections(Point& c, double r) const {
        Line l = Line::from_points(start, end);
        std::vector<Point> res = l.circle_intersections(c,r);
        for(int i = 0; i<res.size(); ++i) {
            if(!point_in_rect(res[i]))
                res.erase(res.begin() + i);
        }
        return res;
    }
};

#endif
