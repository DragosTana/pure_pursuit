#include "PurePursuit.hpp"

PurePursuit::PurePursuit(double horizon, double wheel_base) :
    horizon_(horizon),
    wheel_base_(wheel_base),
    last_closest_index_(0),
    fractional_lookahead_index_(0.0), 
    last_goal_point_(0.0, 0.0) {}

int PurePursuit::closest_point(const Point& position, const std::vector<Point>& path) {
    
    double distance = __DBL_MAX__;
    for (int i = last_closest_index_; i < path.size(); i++) {
        if (position.distance(path[i]) < distance) {
            distance = position.distance(path[i]);
            last_closest_index_ = i;
        }
    }
    return last_closest_index_;
}

Point PurePursuit::goal_point(const Point& position, const std::vector<Point>& path) {
    
    for (int i = this->last_closest_index_; i < path.size() - 1; i++) {
        Point segment_start = path[i];
        Point segment_end = path[i + 1];

        Point d = segment_end - segment_start;
        Point f = segment_start - position;

        double a = d.dot(d);
        double b = 2 * f.dot(d);
        double c = f.dot(f) - this->horizon_ * this->horizon_;

        double discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            discriminant = sqrt(discriminant);

            // Calculate t values
            double t1 = (-b - discriminant) / (2 * a);
            double t2 = (-b + discriminant) / (2 * a);

            if (t1 >= 0 && t1 <= 1) {
                // Calculate intersection point
                Point intersection_point = segment_start + d * t1; // LOL LA MOLTIPLICAZIONE NON E SIMMETRICA RISPETTO ALL'OPERATOR OVERLOADING

                // Check if the intersection point is ahead of the last lookahead point
                if (i + t1 > fractional_lookahead_index_) {
                    fractional_lookahead_index_ = i + t1;
                    last_goal_point_ = intersection_point;
                    return intersection_point;
                }
            }

            if (t2 >= 0 && t2 <= 1) {
                // Calculate intersection point
                Point intersection_point = segment_start + d * t2;

                // Check if the intersection point is ahead of the last lookahead point
                if (i + t2 > fractional_lookahead_index_) {
                    fractional_lookahead_index_ = i + t2;
                    last_goal_point_ = intersection_point;
                    return intersection_point;
                }
            }
        }
    }    

    // If no valid lookahead point is found, use the last lookahead point
    return path[last_closest_index_];
}

double PurePursuit::calculate(const Point& position, const Point& goal_point, double yaw) {
    double alpha = atan2(goal_point.y - position.y, goal_point.x - position.x) - yaw;
    double teta = atan2(2.0 * this->wheel_base_ * sin(alpha), this->horizon_);
    return teta;
}