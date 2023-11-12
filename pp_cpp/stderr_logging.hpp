#ifndef STDERR_LOGGING_H
#define STDERR_LOGGING_H

#include<iostream>
#include "geometry.hpp"

void printerr_point(std::string preamble, const Point &p,
                    std::string postamble="", bool endline=true) {
    std::cerr << preamble
              << p.x << "," << p.y
              << postamble;
    if(endline)
        std::cerr<<std::endl;
}

void printerr_segment(std::string preamble, const Segment &s,
                      std::string postamble="", bool endline=true) {
    std::cerr << preamble
              << s.start.x << "," << s.start.y
              << " - "
              << s.end.x<<","<<s.end.y
              << postamble;
    if(endline)
        std::cerr<<std::endl;
}

#endif
