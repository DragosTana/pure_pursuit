#ifndef PURE_PURSUIT_CLASS_H
#define PURE_PURSUIT_CLASS_H

#include <chrono>
#include <thread>

#include "geometry.hpp"
#include "utils.hpp"

#include "stderr_logging.hpp"

#define DEBUGGING
/*
 * PurePursuit class
 * This class implements the pure pursuit algorithm for path following
 */
class PurePursuit {
private:
    int last_visited = 0;
    double horizon; 
    double wheel_base;
    Point position;
    const std::vector<Point>& path;

    // TODO questa cosa fa cagare, ma non so come altro segnalare "non ho trovato"
    static const Point error_point;

    bool is_valid_point(const Point& p) const;
    Point next_in_segment(const Segment& seg);
    Point next_in_path();
    void wee_wee_move(const Point& goal);
    void print_status(Point goal);

public:
    PurePursuit(float radius, float wheel_base,
                const Point &position, const std::vector<Point> &path);

    int frame_id = 0;
    void wee_wee_step();
    void wee_wee_path(int ignored);
};

#undef DEBUGGING

#endif

