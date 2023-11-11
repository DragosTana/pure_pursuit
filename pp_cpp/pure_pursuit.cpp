#include <bits/types/error_t.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <string>
#include <thread>
#include <limits>
#include "utils.hpp"
#include "geometry.hpp"
#include "geometry_competitive.hpp"

#define DEBUGGING
/*
 * PurePursuit class
 * This class implements the pure pursuit algorithm for path following
 */
enum intersect_status {INSIDE, OUTSIDE, CUTTING};

class PurePursuit {
private:
    int last_visited = 0; // suona decisamente meglio di last_found_index
    double horizon; 
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

    Point next_in_segment(const Segment& seg) {
        // std::vector<Point> intersects = segment_intersections(seg, position, horizon);
        // std::vector<Point> intersects = seg.circle_intersections(position, horizon);
        std::vector<Point> intersects = competitive_segment_intersects(seg, position, horizon);
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
        const auto next_index=[this](const int i) {
            return (i+1)%path.size();
            // return i+1;
        };

        const auto prev_index=[this](const int i) {
            return (i-1+path.size())%path.size();
        };

        const int initial_last_visited = last_visited;

        Point goal_candidate = error_point; // valore iniziale a cazzo
        for(int i = initial_last_visited ;
            i != (int)prev_index(initial_last_visited) ;
            i = next_index(i))
            {   
                Segment segment = Segment(path[i], path[next_index(i)]);
                // il verso segment.start, segment.end è usato localmente come nozione di "in avanti"
                goal_candidate = next_in_segment(segment);
                bool intersect_found = is_valid_point(goal_candidate);

                // pick candidate
                if (intersect_found) {
                    std::cerr<<"intersect found"<<std::endl;
                    printerr_point("goal point ",
                                   goal_candidate,
                                   " intersects the segment");
                } else {
                    std::cerr<<"intersect NOT found"<<std::endl;
                    printerr_point("goal point ",
                                   goal_candidate,
                                   " doesn't intersect the segment");
                    printerr_point("trying to go back to the path, using segment start (",
                                   segment.start ,
                                   ") as goal candidate");
                    last_visited = next_index(i);
                    goal_candidate = segment.end;
                    break;
                }

                // evaluate candidate
                if (segment.end.is_closer(goal_candidate, position)) {
                    printerr_point("candidate point ",
                                   goal_candidate,
                                   " is good goal point, goes forward");
                    // candidato tra i e i+1
                    last_visited = i;
                    break; // will use candidate after the loop
                } else {
                    printerr_point("candidate point " ,
                                   goal_candidate,
                                   "is not a good point, goes backwards");
                    // avoid going backwards, consider the next segment
                    last_visited = next_index(i);
                    continue;
                }
            }
        return goal_candidate;
    }

    void wee_wee_move(const Point& goal) {
        position.x = (position.x + goal.x)/2;
        position.y = (position.y + goal.y)/2;
    }

    void print_status(Point goal) {
        std::cout<<position.x<<':'<<position.y
                 <<':'<<goal.x<<':'<<goal.y
                 <<':'<<horizon<<std::endl;

#ifdef DEBUGGING
        printerr_point("position : ", position);
        printerr_point("goal : ", goal);
        std::cerr<< "horizon : " << horizon << std::endl;
        std::cerr<< "distance from goal : " << position.distance(goal) << std::endl;
        std::cerr<< "last visited index : " << last_visited << std::endl;
        printerr_point("path [last_visited] : ", path[last_visited]);
        printerr_segment("segmento figo : ", Segment(path[last_visited],
                                                     path[last_visited+1]));
#endif
    }


    bool is_inside_radius(const Point& p) {
        // fanculo la radice quadrata
        return position.distance_square(p) <= horizon*horizon;
    }

public:
#ifdef DEBUGGING
    int frame_id = 0;
#endif

    PurePursuit(float radius, float wheel_base, const Point &position, const std::vector<Point> &path)
        : horizon(radius), wheel_base(wheel_base),
        position(position.x, position.y),
        path(path)
    {}

    void wee_wee_step() {
#ifdef DEBUGGING
        std::cerr<<"{{{ FRAME "<< frame_id << " TROVA PUNTI }}}"<<std::endl;
#endif
        Point goal = next_in_path();
#ifdef DEBUGGING
        std::cerr<<"{{{ FRAME "<< frame_id << " STATO MACCHINA }}}"<<std::endl;
#endif
        print_status(goal);
        wee_wee_move(goal);
#ifdef DEBUGGING
        std::cerr<<"{{{ FINE FRAME "<< frame_id++ << " }}}"<<std::endl;
#endif
    }

    void wee_wee_path(int ignored) {
        for(int i = 0; i<ignored; ++i) {
            wee_wee_step();
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
                    
        while(true) {
            wee_wee_step();
            std::cin.ignore(1);
        }
    }
};

double random_offset(double max) {
    return max*((double)std::rand()/(double)RAND_MAX);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "per chiamare fai ./pp <file> <look ahead> <wheel base> [<ignored>]"<< std::endl;
        return 1;
    }
    int ignored = 0;
    if (argc == 5) {
        ignored=std::stoi(argv[4]);
    }
    std::string waypoints_filename = argv[1];
    double look_ahead = std::stod(argv[2]);
    double wheel_base = std::stod(argv[3]);
    std::vector<Point> path = load_csv(waypoints_filename);

    std::srand(std::time(nullptr));
    Point starting_position = Point(path[0].x + random_offset(1),
                                    path[0].y + random_offset(1));
    PurePursuit pp(look_ahead, wheel_base, starting_position, path);
    pp.wee_wee_path(ignored);
    return 0;
}

