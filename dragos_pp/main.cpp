#include "utils.hpp"
#include "PurePursuit.cpp"

int main(int argc, char** argv) {
    //read horizon from command line
    double horizon = 0.0;
    horizon = atof(argv[1]);
    std::vector<Point> path = load_csv("waypoints.csv");
    PurePursuit pp(horizon, 4.0);
    Point position = Point(path[0].x + random_offset(1), path[0].y + random_offset(1));

    for (int i = 0; i < 100; i++) {
        Point goal = pp.goal_point(position, path);
        // mandiamo messaggio secondo le giuste specifiche
        std::cout << position.x << ":" << position.y << ":" << goal.x << ":" << goal.y << ":" << horizon << std::endl;

        position = (position + goal)/2;
    }
}