#ifndef PP_CPP_UTILS_H
#define PP_CPP_UTILS_H

/**
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * FANCULO PRAGMA ONCE
 * @param fanculo
 * @return pragma once 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "geometry.hpp"

int sign(int n) {
    if(n>=0)
        return 1;
    return -1;
}

std::vector<Point> load_csv (const std::string& file_name){
    /**
     * loads vector of points from pandas csv
     */
    std::vector<Point> points;
    std::ifstream file;
    file.open(file_name, std::ios::in);
    std::string line, word;

    // ignora l'header (dataframe pandas)
    getline(file,line);

    if(file.is_open()){
        //std::cout<<"file is opened"<<std::endl;
        while(getline(file, line)){
            std::stringstream str(line);
            // qui si assume che il csv sia a due colonne
            getline(str, word, ',');
            double p_x = std::stod(word);
            getline(str, word, ',');
            double p_y = std::stod(word);

            points.push_back(Point(p_x, p_y));
        }
    }
    else std::cout<<"ERROR: couldn't open file"<<std::endl;

    return points;
}

#endif
