#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
/*
* Point class
* This class implements a point in 2D space
*/
class Point {
public:
    double x;
    double y;

    Point(double x, double y): x(x), y(y) {}

    double distance(const Point& other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }

    double dot(const Point& other) {
        return x * other.x + y * other.y;
    }

    Point operator+(const Point& other) {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) {
        return Point(x - other.x, y - other.y);
    }

    Point operator*(const double& scalar) {
        return Point(x * scalar, y * scalar);
    }

    Point operator/(const double& scalar) {
        return Point(x / scalar, y / scalar);
    }


};

#endif