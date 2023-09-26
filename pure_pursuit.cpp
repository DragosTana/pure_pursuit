#include<vector>
#include<iostream>
#include<cmath>


class PurePursuit{
    public:

    float radius_; 
    float wheel_base_;
    
    PurePursuit(float radius, float wheel_base)
    {
        radius_ = radius; // Maybe this must be dynamic ?
        wheel_base_ = wheel_base;
    }

    private:

    float distance(const float &x1, const float &y1, const float &x2, const float &y2){
        return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
    }

    float squared_distance(const float &x1, const float &y1, const float &x2, const float &y2){
        return pow((x2-x1),2) + pow((y2-y1),2);
    }

    std::pair<double, double> find_intersection(const std::vector<std::pair<float, float>>& coordinates, const std::pair<float, float>& position){

        // find all the points in coordinates that are within the radius of the car
        std::vector<std::pair<float, float>> points_in_radius;
        for(auto point : coordinates){
            if(squared_distance(point.first, point.second, position.first, position.second) <= pow(radius_,2)){
                points_in_radius.push_back(point);
            }
        }
    
        // find all the intersections between the circle and the lines between the points
        std::vector<std::pair<float, float>> intersections;
        for (size_t i = 1; i < points_in_radius.size(); i++){
            float x1 = points_in_radius[i-1].first;
            float y1 = points_in_radius[i-1].second;
            float x2 = points_in_radius[i].first;
            float y2 = points_in_radius[i].second;

            float dx = x2 - x1;
            float dy = y2 - y1;
            float dr = sqrt(pow(dx,2) + pow(dy,2));
            float D = x1*y2 - x2*y1;

            float discriminant = pow(radius_,2)*pow(dr,2) - pow(D,2);
            if(discriminant >= 0){
                float x1_ = (D*dy + (dy/fabs(dy))*dx*sqrt(discriminant))/pow(dr,2);
                float x2_ = (D*dy - (dy/fabs(dy))*dx*sqrt(discriminant))/pow(dr,2);
                float y1_ = (-D*dx + fabs(dy)*sqrt(discriminant))/pow(dr,2);
                float y2_ = (-D*dx - fabs(dy)*sqrt(discriminant))/pow(dr,2);
            }
        }

    }

    //float steering_angle()

}