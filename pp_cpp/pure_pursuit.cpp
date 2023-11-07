#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <string>
#include <thread>
#include <limits>
#include "utils.hpp"
#include "geometry.hpp"

#define DEBUGGING 1
/*
    ./pp_cpp/pp waypoints.csv 4 2 | python3 pp_python/cpp_visualization.py
    NOTE:
    passare a float invece di double?
*/

/*
* PurePursuit class
* This class implements the pure pursuit algorithm for path following
*/
class PurePursuit {
private:
    int last_visited = 0; 
    double radius; 
    double wheel_base;
    Point position;
    const std::vector<Point>& path;

    // TODO questa cosa fa cagare, ma non so come altro segnalare "non ho trovato"
    const Point error_point = Point(MAXFLOAT, MAXFLOAT);
    const double some_threshold = 10000.0;

    /*
    * Returns true if the point is valid, false otherwise
    * @param[in] p the point to check
    */
    bool is_valid_point(const Point& p) const {
        return
            (fabs(p.x) <= some_threshold) &&
            (fabs(p.y) <= some_threshold);
    }

    std::vector<Point> segment_intersections(const Segment& seg) {

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
            float sol_x1 = ((D * dy) + (sign(dy) * dx * sqrt(discriminant))) / pow(dr,2);
            float sol_x2 = ((D * dy) - (sign(dy) * dx * sqrt(discriminant))) / pow(dr,2);
            float sol_y1 = (-(D * dx) + fabs(dy) * sqrt(discriminant)) / pow(dr,2);
            float sol_y2 = (-(D * dx) - fabs(dy) * sqrt(discriminant)) / pow(dr,2);

            Point sol1 = Point(sol_x1 + position.x, sol_y1 + position.y);
            Point sol2 = Point(sol_x2 + position.x, sol_y2 + position.y);

            // potrebbe non essere il non plus ultra dell'efficienza
            // possibili altri metodi?

            if (seg.point_in_rect(sol1)) {
                solutions.push_back(sol1);
            }
            if (seg.point_in_rect(sol2)) {
                solutions.push_back(sol2);      
            }

#ifdef DEBUGGING
            printerr_point("segment start", seg.start);
            printerr_point("segment end", seg.end);
            printerr_point("solution 1", sol1);
            printerr_point("solution 2", sol2);
            std::cerr<<"last visited : "<<last_visited<<std::endl;

            std::cerr<< "==== BEGIN SOLUTIONS ===="<<std::endl;
            if(solutions.size()) {
                for (int i = 0; i < solutions.size(); ++i) {
                    printerr_point("solutions[" + std::to_string(i) + "]", solutions[i]);
                }
            } else {
                std::cerr << "{no solutions}" << std::endl;
            }
            std::cerr<< "===== END SOLUTIONS ====="<<std::endl;
#endif
        }
        return solutions;
    }

    Point next_in_segment(const Segment& seg) {
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

        auto next_ind = [this](const int i) {return (i+1)%path.size();};

        for(int i = this->last_visited, n_visited = 0; 
            n_visited != path.size(); 
            i = next_ind(i), ++n_visited)
            {   
                
                Segment segment = Segment(path[i], path[next_ind(i)]);
                Point segment_goal = next_in_segment(segment);
#ifdef DEBUGGING
                printerr_point("segment goal", segment_goal);
#endif
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
                    // this->last_visited = next_ind(i);
                    return path[this->last_visited];
                }
            }
        std::cerr<<"in the end : last found " << last_visited << std::endl;
        return error_point;
    }

    Point goal_point(){

        auto next_ind = [this](const int i) {return (i+1)%path.size();};

        for (int i = this->last_visited, n_visited = 0; n_visited != path.size(); i = next_ind(i), ++n_visited) {
            std::cerr<<"i : "<<i<<std::endl;
            Segment segment = Segment(path[i], path[next_ind(i)]);
            Point segment_goal = next_in_segment(segment);
            if (is_valid_point(segment_goal)) {
                // intersection found with segment
                if (segment.end.is_closer(position, segment_goal)) {
                    //this->last_visited = next_ind(i);
                    std::cerr<<"case in which goal is behind the position"<<std::endl;
                    continue;
                } else {
                    std::cerr<<"trovato goal"<<std::endl;
                    this->last_visited = i;
                    return segment_goal;
                }
            } else {
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
        std::cout<<position.x<<':'<<position.y
                <<':'<<goal.x<<':'<<goal.y
                <<':'<<radius<<std::endl;

#ifdef DEBUGGING
        printerr_point("position : ", position);
        printerr_point("goal", goal);
        std::cerr<<"================ [ FINE FRAME ] ================"<<std::endl;
#endif
    }

    void printerr_point(const std::string name, const Point &p) const {
        std::cerr<<name<< " - coorrds : "<<p.x<<':'<<p.y<<std::endl;
    }

    void wee_wee_path() {
        while(true) {
            Point goal = goal_point();
            print_status(goal);
            wee_wee_move(goal);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

double random_offset(double max) {
    return max*((double)std::rand()/(double)RAND_MAX);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "per chiamare fai ./pp <file> <look ahead> <wheel base>"<< std::endl;
        return 1;
    }
    std::string waypoints_filename = argv[1];
    double look_ahead = std::stod(argv[2]);
    double wheel_base = std::stod(argv[3]);
    std::vector<Point> path = load_csv(waypoints_filename);

    std::srand(std::time(nullptr));
    Point starting_position = Point(path[0].x + random_offset(1),
                                    path[0].y + random_offset(1));
    PurePursuit pp(look_ahead, wheel_base, starting_position, path);
    pp.wee_wee_path();
    return 0;
}

