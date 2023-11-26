#ifndef PURE_PURSUIT_CLASS_H
#define PURE_PURSUIT_CLASS_H

#include <vector>
#include "geometry.hpp"

/*
 * PurePursuit class
 * This class implements the pure pursuit algorithm for path following
 * implementation based on doc/pure_pursuit.pdf
 */
class PurePursuit {
public:
    PurePursuit(double horizon, double wheel_base);

    /*
    * Calculates the steering angle for the robot
    * @param position: current position of the robot
    * @param path: path to follow
    * @return steering angle
    */
    double calculate(const Point& position, const Point& goal_point, double yaw = 0.0);

    /*
    * Calculates the lookahead point on the path
    * @param position: current position of the robot
    * @param path: path to follow
    * @return lookahead point    
    */
    Point goal_point(const Point& position, const std::vector<Point>& path); 

private:
    double horizon_;
    double wheel_base_;
    int last_closest_index_;
    double fractional_lookahead_index_;
    Point last_goal_point_;

    /*
    * Calculates the closest point on the path to the robot
    * @param position: current position of the robot
    * @param path: path to follow
    * @return index of the closest point on the path
    */
    int closest_point(const Point& position, const std::vector<Point>& path);

};

#endif