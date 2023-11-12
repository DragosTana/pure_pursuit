#include "PurePursuit.hpp"

#define DEBUGGING

const Point PurePursuit::error_point = Point(MAXFLOAT, MAXFLOAT);

/*
 * Returns true if the point is valid, false otherwise
 * @param[in] p the point to check
 */
bool PurePursuit::is_valid_point(const Point& p) const {
    const double some_threshold = 10000.0;
    return
        (fabs(p.x) <= some_threshold) &&
        (fabs(p.y) <= some_threshold);
}

Point PurePursuit::next_in_segment(const Segment& seg) {
    std::vector<Point> intersects = seg.circle_intersects(Circle(position, horizon));
    switch(intersects.size()) {
    case 1:
        return intersects[0];
    case 2:
        return seg.end.closest_same_segment(intersects[0], intersects[1]);
    default:
        return error_point;
    }
}


Point PurePursuit::next_in_path() {
    const auto next_index=[this](const int i) {
        return (i+1)%path.size();
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

void PurePursuit::wee_wee_move(const Point& goal) {
    position.x = (position.x + goal.x)/2;
    position.y = (position.y + goal.y)/2;
}

void PurePursuit::print_status(Point goal) {
        std::cout<<position.x<<':'<<position.y
                 <<':'<<goal.x<<':'<<goal.y
                 <<':'<<horizon<<std::endl;

        printerr_point("position : ", position);
        printerr_point("goal : ", goal);
        std::cerr<< "horizon : " << horizon << std::endl;
        std::cerr<< "distance from goal : " << position.distance(goal) << std::endl;
        std::cerr<< "last visited index : " << last_visited << std::endl;
        printerr_point("path [last_visited] : ", path[last_visited]);
        printerr_segment("segmento figo : ", Segment(path[last_visited],
                                                     path[last_visited+1]));
    }

PurePursuit::PurePursuit(float radius, float wheel_base, const Point &position, const std::vector<Point> &path)
    : horizon(radius), wheel_base(wheel_base),
      position(position.x, position.y),
      path(path)
{}


void PurePursuit::wee_wee_step() {
        std::cerr<<"{{{ FRAME "<< frame_id << " TROVA PUNTI }}}"<<std::endl;
        Point goal = next_in_path();
        std::cerr<<"{{{ FRAME "<< frame_id << " STATO MACCHINA }}}"<<std::endl;
        print_status(goal);
        wee_wee_move(goal);
        std::cerr<<"{{{ FINE FRAME "<< frame_id++ << " }}}"<<std::endl;
    }


void PurePursuit::wee_wee_path(int ignored) {
    for(int i = 0; i<ignored; ++i) {
        wee_wee_step();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
                    
    while(true) {
        wee_wee_step();
        std::cin.ignore(1);
    }
}

#undef DEBUGGING
