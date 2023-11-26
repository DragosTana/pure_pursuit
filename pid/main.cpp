#include <iostream>
#include <jsoncpp/json/value.h>
#include <fstream>
#include <string>
#include <math.h>
#include "PID.hpp"


// to compile this file:
// g++ -std=c++11 -o pid main.cpp PID.cpp -I/usr/include/jsoncpp -ljsoncpp

int main() {
    //std::ifstream parameters("parameters.json", std::ifstream::binary);
    //Json::Value param;
    //parameters >> param;
    //double kp = std::stod(param["kp"]);
    //double ki = std::stod(param["ki"]);
    //double kd = std::stod(param["kd"]);
    //double min_output = std::stod(param["min_output"]);
    //double max_output = std::stod(param["max_output"]);
    //double low_pass_cutoff_hz = std::stod(param["low_pass_cutoff_hz"]);

    double kp = 1;
    double ki = 0.1;
    double kd = 0.5;
    double min_output = -7.0;
    double max_output = 7.0;
    double low_pass_cutoff_hz = 0;

    PIDController pid_controller(kp, ki, kd, min_output, max_output, low_pass_cutoff_hz);

    double current_value = 50.0;   
    size_t count = 300; 
    double setpoint[count];
    double random_noise = 0.0;

    for (size_t i = 0; i < count; i++) {   
        if (i < 25) {setpoint[i] = 50.0;}
        else if (i < 75) {setpoint[i] = 100.0;}
        else {setpoint[i] = 50.0;}
    }
    
    for (size_t i = 0; i < count; ++i) {
        double output = pid_controller.calculate(setpoint[i], current_value);
        current_value += output;
        // add random noise to signal
        // random_noise = (rand() % 5)/1.0;
        current_value += random_noise;
        std::cout << i << ":" << current_value << std::endl;
        std::cerr << random_noise << std::endl;
        // sleep(0.01) <-- NOTE: ADD THIS!
    }

    return 0;

}


/*
int main() {
    // Create PID controller
    PIDController pid_controller(kp, ki, kd, min_output, max_output, derivative_filter_tau);

    // Simulate PID control on an acceleration actuator
    double setpoint = 50.0;
    double current_value = 0.0;

    // Simulation loop
    for (int i = 0; i < 100; ++i) {
        // Calculate PID controller output
        double output = pid_controller.Calculate(setpoint, current_value);

        // Simulate acceleration actuator (e.g., first-order system)
        double acceleration_system = 1.0 / (1.0 + 0.1) * output;

        // Update current_value
        current_value = acceleration_system;


        // Print results
        std::cout << ", Output: " << output << std::endl;
    }

    return 0;
}
*/
