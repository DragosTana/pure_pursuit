#ifndef PURE_PURSUIT_CLASS_H
#define PURE_PURSUIT_CLASS_H

#include <vector>
#include "geometry.hpp"

#define DEBUGGING
/*
 * PurePursuit class
 * This class implements the pure pursuit algorithm for path following
 * 
 * implementation based on file:///home/dragos/Downloads/be0e06de00e07db66f97686505c3f4dde2e332dc.pdf
 */
class PurePursuit {
public:
    PurePursuit(double horizon, double wheel_base);

    /*
    * Calculates the steering angle for the robot
    *
    * @param position: current position of the robot
    * @param path: path to follow
    * 
    * @return steering angle
    */
    double calculate(const Point& position, const Point& goal_point);

    /*
    * Calculates the lookahead point on the path
    * 
    * @param position: current position of the robot
    * @param path: path to follow
    * 
    * @return lookahead point    
    */
    Point goal_point(const Point& position, const std::vector<Point>& path); //! NOTE: this should be a private mehtod but is public for testing purposes

private:
    double horizon_;
    double wheel_base_;
    int last_closest_index_;
    double fractional_lookahead_index_;

    /*
    * Calculates the closest point on the path to the robot
    *
    * @param position: current position of the robot
    * @param path: path to follow
    *   
    * @return index of the closest point on the path
    */
    int closest_point(const Point& position, const std::vector<Point>& path);


};
#undef DEBUGGING

#endif