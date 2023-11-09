#ifndef PP_CPP_GEOMETRY_H
#define PP_CPP_GEOMETRY_H

#include<vector>
#include<cmath>
#include<iostream>

bool in_range(double a, double ex1, double ex2) {
    // memento, (a <= b <= c) NON funziona in c++
    return ((ex1 <= a) && (a <= ex2)) || ((ex2 <= a) && (a <= ex1));
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
};




#endif
