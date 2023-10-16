#pragma once
#ifndef UTILS_H
#define UTILS_H

#include 'point.h'
#include <vector>


std::vector<Point> load_csv (const std::string& file_name){

    std::vector<Point> points;
    std::ifstream file;
    file.open(file_name, std::ios::in);
    std::string line, word;

    if(file.is_open()){
        //std::cout<<"file is opened"<<std::endl;
        while(getline(file, line)){
            std::stringstream str(line);
            Point p;
            int i = 0;
            float coordinates[2] = {0,0};
            while(getline(str, word, ',')){
                    coordinates[i] = std::stod(word);
                    i++;
            }
            p.x = coordinates[0];
            p.y = coordinates[1];
            points.push_back(p);
        }
    }
    else std::cout<<"ERROR: couldn't open file"<<std::endl;

    return points;
}

int sign(int n) {
    if(n>=0)
        return 1;
    return -1;
}

#endif // UTILS_H