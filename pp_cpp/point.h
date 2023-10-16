#pragma once
#ifndef POINT_H
#define POINT_H

#include<limits.h>
#include<math.h>

class Point {
public:
    float x;
    float y;

    Point(float x, float y):x(x),
                            y(y) {}

    float distance(const Point& other) const {
        return sqrt(distance_square(other));
    }

    float distance_square(const Point& other) const {
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

    Segment(const Point& start, const Point& end):start(start.x, start.y),
                                                  end(end.x, end.y) {}

    const bool point_in_rect(const Point& p) const {
        return
            (start.x <= p.x) && (p.x <= end.x) &&
            (start.y <= p.y) && (p.y <= end.y);
    }
};

#endif // POINT_H