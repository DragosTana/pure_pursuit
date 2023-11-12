#include <bits/types/error_t.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <string>
#include <limits>
#include "utils.hpp"
#include "geometry.hpp"
#include "PurePursuit.hpp"


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

