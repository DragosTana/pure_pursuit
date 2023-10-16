#include<vector>
#include<iostream>
#include<cmath>
#include<limits>

/* devo farmi una ripassata di come cazzo si usa const nelle definizioni di funzione
 * non ci sto capendo una sega
 */

int sign(int n) {
    if(n>=0)
        return 1;
    return -1;
}

class Point {
public:
    float x;
    float y;

    Point(float x, float y):x(x),
                            y(y) {}

    float distance(const Point& other) const {
        return sqrt(distance_square(other));
    }

    float distance_square(const Point& other) const {
        return pow((x - other.x), 2) + pow((y - other.y), 2);
    }

    bool is_closer(const Point& closer, const Point& farther) const {
        return distance_square(closer) <= distance_square(farther);
    }

    bool is_closer_same_segment(const Point& closer, const Point& farther) const {
        if(closer.x == farther.x) {
            return fabs(y-closer.y) <= fabs(y-farther.y);
        }
        return fabs(x-closer.x) <= fabs(x-farther.x);
    }

    Point closest(const Point& p1, const Point& p2) const {
        if(is_closer(p1, p2))
            return p1;
        return p2;
    }

    Point closest_same_segment(const Point& p1, const Point& p2) const {
        if(is_closer_same_segment(p1, p2))
            return p1;
        return p2;
    }
};

class Segment {
public:
    Point start;
    Point end;

    Segment(const Point& start, const Point& end):start(start.x, start.y),
                                                  end(end.x, end.y) {}

    const bool point_in_rect(const Point& p) const {
        return
            (start.x <= p.x) && (p.x <= end.x) &&
            (start.y <= p.y) && (p.y <= end.y);
    }
};

class PurePursuit {
private:
    int last_visited = 0; // suona decisamente meglio di last_found_index
    float radius; 
    float wheel_base;
    Point position;

    // TODO questa cosa fa cagare, ma non so come altro segnalare "non ho trovato"
    const Point error_point = Point(MAXFLOAT, MAXFLOAT);
    bool is_valid_point(const Point& p) const {
        return
            (p.x != error_point.x) ||
            (p.y !=error_point.y);
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
    Point next_in_path(const std::vector<Point>& path_points) {
        auto next_ind=[path_points](const int i) {
            return (i+1)%path_points.size();
        };
        for(int i = this->last_visited, n_visited = 0;
            n_visited != path_points.size();
            i = next_ind(i), ++n_visited)
            {
                Segment segment = Segment(path_points[i], path_points[next_ind(i)]);
                Point segment_goal = next_in_segment(seg);
            
                if(is_valid_point(segment_goal)) {
                    // intersection found with segment
                    if(segment.end.is_closer(position, segment_goal)) {
                        this->last_visited = next_ind(i);
                        continue;
                    }
                    else {
                        this->last_visited = i;
                        return segment_goal;
                    }
                }
                else {
                    // no intersection found with segment
                    this->last_visited = next_ind(this->last_visited);
                    return path_points[this->last_visited];
                }
            }
        return error_point;
    }

public:
    PurePursuit(float radius, float wheel_base, const Point& position):
        radius(radius),
        wheel_base(wheel_base),
        position(position.x, position.y) {}

    void print_status(Point goal) {
        std::cout<<position.x<<':'<<position.y
                 <<':'<<goal.x<<':'<<goal.y
                 <<':'<<radius;
    }
    void wee_wee_path(std::vector<Point>& path) {
        return;
    }
};

int main() {
    std::cout<<"ei fu, siccome immobile"<<std::endl;
    return 0;
}
