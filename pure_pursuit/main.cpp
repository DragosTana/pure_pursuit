#include "utils.hpp"
#include "PurePursuit.hpp"

// TO COMPILE: g++ -std=c++11 main.cpp PurePursuit.cpp -o pp
// TO RUN: ./pp <horizon> 
// TO RUN AND VISUALIZE: .pure_pursuit/pp <horizon> | python3 visualization/pp_visual.py
// for example: ./pure_pursuit/pp 6 | python3 visualization/pp_visual.py

int main(int argc, char** argv) {
    //read horizon from command line
    double horizon = 0.0;
    horizon = atof(argv[1]);
    std::vector<Point> path = load_csv("waypoints.csv");
    PurePursuit pp(horizon, 4.0);
    Point position = Point(path[0].x + random_offset(1), path[0].y + random_offset(1));
    Point goal(0.0, 0.0);

    for (int i = 0; i < 500; i++) {
        goal = pp.goal_point(position, path);
        // mandiamo messaggio secondo le giuste specifiche
        std::cout << position.x << ":" << position.y << ":" << goal.x << ":" << goal.y << ":" << horizon << std::endl;
        std::cerr << position.x << ":" << position.y << ":" << goal.x << ":" << goal.y << ":" << horizon << std::endl;

        position = (position + goal)/2;
        position.x += random_offset(0.5);
        position.y += random_offset(0.5);
    }
}