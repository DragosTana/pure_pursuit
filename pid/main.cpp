#include <iostream>
#include <jsoncpp/json/value.h>
#include <fstream>
#include <string>
#include "PID.hpp"

int main() {
    // std::ifstream parameters("parameters.json", std::ifstream::binary);
    // Json::Value param;
    // parameters >> param;
    // double kp = std::stod(param["kp"]);
    // double ki = std::stod(param["ki"]);
    // double kd = std::stod(param["kd"]);
    // double min_output = std::stod(param["min_output"]);
    // double max_output = std::stod(param["max_output"]);

    double kp = 1;
    double ki = 0.1;
    double kd = 0.1;
    double min_output = -10.0;
    double max_output = 10.0;

    PIDController pid_controller(kp, ki, kd, min_output, max_output);

    double current_value = 50.0;    
    double setpoint[100];
    size_t count = 100;

    for (size_t i = 0; i < count; i++) {   
        if (i < 25) {setpoint[i] = 50.0;}
        else if (i < 75) {setpoint[i] = 100.0;}
        else {setpoint[i] = 50.0;}
    }
    
    for (size_t i = 0; i < count; ++i) {
        double output = pid_controller.Calculate(setpoint[i], current_value);
        current_value += output;
        std::cout << i << ":" << current_value << std::endl;
    }

    return 0;
}
