#include "utils.hpp"
#include "geometry.hpp"

std::vector<Point> segment_intersections(const Segment& seg, const Point& position, const float radius) {

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

int main() {
    Point start = Point(17.2344, 91.9884);
    Point end = Point(27.9448, 92.7779);
    Segment seg = Segment(start, end);
    Point position = Point(12.4021, 90.7138);
    float radius = 4.0;

    std::vector<Point> solutions = segment_intersections(seg, position, radius);
    std::cout << solutions.size() << std::endl;
    for (int i = 0; i < solutions.size(); ++i) {
        std::cout << solutions[i].x << " " << solutions[i].y << std::endl;
    }
}