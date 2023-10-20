#include <cmath>
#include <cstdlib>
#include<iostream>
#include <thread>
#include <limits>
#include "utils.hpp"
#include "geometry.hpp"

class PurePursuit {
private:
    int last_visited = 0; // suona decisamente meglio di last_found_index
    float radius; 
    float wheel_base;
    Point position;
    const std::vector<Point>& path;

    // TODO questa cosa fa cagare, ma non so come altro segnalare "non ho trovato"
    const Point error_point = Point(MAXFLOAT, MAXFLOAT);
    const double some_threshold = 10000.0;
    bool is_valid_point(const Point& p) const {
        return
            (fabs(p.x) <= some_threshold) &&
            (fabs(p.y) <= some_threshold);
    }

    std::vector<Point> segment_intersections(const Segment& seg) const {
        // questo codice è stato copiato e modificato
        // dalla versoine python perchè sono una testa di cazzo
        float x1_off = seg.start.x - position.x;
        float y1_off = seg.start.y - position.y;
        float x2_off = seg.end.x - position.x;
        float y2_off = seg.end.y - position.y;

        float dx = x2_off - x1_off;
        float dy = y2_off - y1_off;

        float dr = sqrt(pow(dx,2) + pow(dy,2));
        float D = (x1_off*y2_off) - (x2_off*y1_off);
        float discriminant = (pow(radius,2)) * pow(dr,2) - pow(D,2);

        std::vector<Point>solutions = {};
        if (discriminant >= 0) {
            // compute line intersections
            float sol_x1 =
                ((D * dy) + (sign(dy) * dx) * sqrt(discriminant))
                / pow(dr,2);
            float sol_x2 =
                ((D * dy) - (sign(dy) * dx) * sqrt(discriminant))
                / pow(dr,2);
            float sol_y1 =
                ((- D * dx) + fabs(dy) * sqrt(discriminant))
                / pow(dr,2);
            float sol_y2 =
                ((- D * dx) - fabs(dy) * sqrt(discriminant))
                / pow(dr,2);

            Point sol1 = Point(sol_x1 + position.x, sol_y1 + position.y);
            Point sol2 = Point(sol_x2 + position.x, sol_y2 + position.y);

            // potrebbe non essere il non plus ultra dell'efficienza
            // possibili altri metodi?
            if (seg.point_in_rect(sol1))
                solutions.push_back(sol1);

            if (seg.point_in_rect(sol2))
                solutions.push_back(sol2);
        }
        return solutions;
    }
    Point next_in_segment(const Segment& seg) const {
        std::vector<Point> intersects = segment_intersections(seg);
        switch(intersects.size()) {
        case 1:
            return intersects[0];
        case 2:
            return seg.end.closest_same_segment(intersects[0], intersects[1]);
        default:
            return error_point;
        }
    }
    Point next_in_path() {
        auto next_ind=[this](const int i) {
            return (i+1)%path.size();
        };
        for(int i = this->last_visited, n_visited = 0;
            n_visited != path.size();
            i = next_ind(i), ++n_visited)
            {
                Segment segment = Segment(path[i], path[next_ind(i)]);
                Point segment_goal = next_in_segment(segment);
                // std::cout<< "x : " << segment_goal.x << " - y : " << segment_goal.y << std::endl;
            
                if(is_valid_point(segment_goal)) {
                    // intersection found with segment
                    if(segment.end.is_closer(position, segment_goal)) {
                        this->last_visited = next_ind(i);
                        std::cerr<<"fucked the distance, falling back to next boi"<<std::endl;
                        continue;
                    }
                    else {
                        std::cerr<<"distance is not fucked, so we're returning the segment intersect"<<std::endl;
                        this->last_visited = i;
                        return segment_goal;
                    }
                }
                else {
                    std::cerr<<"no segment intersect found, returning next vertex"<<std::endl;
                    // no intersection found with segment
                    this->last_visited = next_ind(this->last_visited);
                    return path[this->last_visited];
                }
            }
        std::cerr<<"in the end : last found " << last_visited << std::endl;
        return error_point;
    }

    void wee_wee_move(const Point& goal) {
        position.x = (position.x + goal.x)/2;
        position.y = (position.y + goal.y)/2;
    }

public:
    PurePursuit(float radius, float wheel_base, const Point &position, const std::vector<Point> &path)
        : radius(radius), wheel_base(wheel_base),
        position(position.x, position.y),
        path(path)
    {}

    void print_status(Point goal) {
        // per python
        std::cout<<position.x<<':'<<position.y 
                <<':'<<goal.x<<':'<< goal.y 
                <<':'<<radius<<std::endl;

        // per noi
        std::cerr<<position.x<<':'<<position.y
                <<':'<<goal.x<<':'<<goal.y
                <<':'<<radius<<std::endl;
    }

    void wee_wee_path() {
        while(true) {
            Point goal = next_in_path();
            print_status(goal);
            wee_wee_move(goal);
            std::cin.ignore();
        }
    }
};

double unit_offset(double max) {
    return max*((double)std::rand()/(double)RAND_MAX);
}

int main() {
    std::srand(std::time(nullptr));
    std::vector<Point> path = load_csv("waypoints.csv");
    PurePursuit pp(0.6, 1, Point(path[0].x + unit_offset(1), path[0].y + unit_offset(1)),
        path);
    pp.wee_wee_path();
    return 0;
}
