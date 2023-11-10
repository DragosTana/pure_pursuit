#include <bits/types/error_t.h>
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

    std::vector<Point> segment_intersections(const Segment& seg) const {
        // questo codice è stato copiato e modificato
        // dalla versoine python perchè sono una testa di cazzo
        double x1_off = seg.start.x - position.x;
        double y1_off = seg.start.y - position.y;
        double x2_off = seg.end.x - position.x;
        double y2_off = seg.end.y - position.y;

        double dx = x2_off - x1_off;
        double dy = y2_off - y1_off;

        double dr = sqrt(pow(dx,2) + pow(dy,2));
        double D = (x1_off*y2_off) - (x2_off*y1_off);
        double discriminant = (pow(horizon,2)) * pow(dr,2) - pow(D,2);

        std::vector<Point>solutions = {};
        if (discriminant >= 0) {
            // compute line intersections
            double sol_x1 =
                ((D * dy) + (sign(dy) * dx * sqrt(discriminant)))
                / pow(dr, 2);
            double sol_x2 =
                ((D * dy) - (sign(dy) * dx * sqrt(discriminant)))
                / pow(dr,2);
            double sol_y1 =
                (-(D * dx) + fabs(dy) * sqrt(discriminant))
                / pow(dr,2);
            double sol_y2 =
                (-(D * dx) - fabs(dy) * sqrt(discriminant))
                / pow(dr,2);

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
            printerr_segment("segment to intersect - ", seg);
            printerr_point("solution 1", sol1);
            printerr_point("solution 2", sol2);
            std::cerr<<"last visited : "<<last_visited<<std::endl;

            std::cerr<<"intersects {";
            if(solutions.size()) {
                for (int i = 0; i < solutions.size(); ++i) {
                    printerr_point(" solutions[" + std::to_string(i) + "]=", solutions[i], "", false);
                }
            } else {
                std::cerr << " none";
            }
            std::cerr<<" }"<<std::endl;
#endif
        }
        return solutions;
    }

    Point next_in_segment(const Segment& seg) {
        std::vector<Point> intersects = segment_intersections(seg);
        // std::vector<Point> intersects = seg.circle_intersections(position, horizon);
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
        };

        const auto prev_index=[this](const int i) {
            return (i-1+path.size())%path.size();
        };

        const int initial_last_visited = last_visited;

        Point goal_candidate = error_point; // valore iniziale a cazzo
        for(int i = initial_last_visited ;
            i != prev_index(initial_last_visited) ;
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
                    last_visited = i;
                    goal_candidate = segment.start;
                    break;
                }

                // evaluate candidate
                if (segment.end.is_closer_same_segment(goal_candidate, position)) {
                    printerr_point("candidate point ",
                                   goal_candidate,
                                   " is good goal point, goes forward");
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
#endif
    }

    void printerr_point(std::string preamble, const Point &p,
                        std::string postamble="", bool endline=true) const {
        std::cerr << preamble
                  << p.x << "," << p.y
                  << postamble;
        if(endline)
            std::cerr<<std::endl;
    }

    void printerr_segment(std::string preamble, const Segment &s,
                          std::string postamble="", bool endline=true) const {
        std::cerr << preamble
                  << s.start.x << "," << s.start.y
                  << " - "
                  << s.end.x<<","<<s.end.y
                  << postamble;
        if(endline)
            std::cerr<<std::endl;
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

    void wee_wee_path() {
        while(true) {
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
            std::cin.ignore(1);
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

