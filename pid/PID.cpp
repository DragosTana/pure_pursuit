#include "PID.hpp"
#include <iostream> 
#include <jsoncpp/json/value.h>
#include <fstream>
#include <string>
/*
    PID Controller
    
    TODO: 

        1. Implement anti-windup on integral term -- FATTOOO 
        2. Implement derivative on measurement
        3. Implemete low pass filter on derivative term  --> IL TAU sarà due volte la frequenza di campionamento
*/
#define DEBUG 1

PIDController::PIDController(double kp, double ki, double kd, double min_output, double max_output) :
    kp_(kp),
    ki_(ki),
    kd_(kd),
    min_output_(min_output),
    max_output_(max_output),
    integral_(0),
    previous_error_(0),
    antiwindup_(false) {}

double PIDController::Calculate(double setpoint, double current_value) {
    double error = setpoint - current_value;
    double output = 0.0;
    double derivative = (error - previous_error_);

    if (antiwindup_ == false) {
        integral_ += error;
        output = kp_ * error + ki_ * integral_ + kd_ * derivative;
    } else {
        output = kp_ * error + kd_ * derivative;
        antiwindup_ = false;
    }

    double clamped_output = clamping(output);
    bool antiwindup_ = antiwindup(output, clamped_output, error);
    previous_error_ = error;

#ifdef DEBUG
    std::cerr << "======================================" << std::endl;
    std::cerr << "error: " << error << std::endl;
    std::cerr << "antiwindup: " << antiwindup_ << std::endl;
    std::cerr << "output: " << output << std::endl;
    std::cerr << "clamped_output: " << clamped_output << std::endl;
#endif

    return clamped_output;
}

double PIDController::clamping(double output) {
    if (output > max_output_) {
        output = max_output_;
    } else if (output < min_output_) {
        output = min_output_;
    }
    return output;
}

bool PIDController::antiwindup(double output, double clamped_output, double error) {
    if ((clamped_output != output) && (error * output > 0)) {
        return true;
    }
    return false;
}
