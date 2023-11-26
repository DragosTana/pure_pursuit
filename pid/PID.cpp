#include "PID.hpp"
#include <iostream> 
#include <jsoncpp/json/value.h>
#include <fstream>
#include <string>
#include <math.h>

#define DEBUG 0

// PID controller constructor
PIDController::PIDController(double kp, double ki, double kd, double min_output, double max_output, double low_pass_cutoff_hz) :
    kp_(kp),
    ki_(ki),
    kd_(kd),
    min_output_(min_output),
    max_output_(max_output),
    low_pass_cutoff_hz_(low_pass_cutoff_hz),
    integral_(0),
    previous_error_(0),
    previous_time_(0),
    previous_derivative_(0),
    antiwindup_(false) {}

double PIDController::calculate(double setpoint, double current_value) {

    double error = setpoint - current_value;
    unsigned long int current_time = nanos();
    double dt = (current_time - previous_time_) / 1e9; 
    double RC = 1.0 / (2.0 * M_PI * low_pass_cutoff_hz_); 
    double output = 0.0;
    
    // Calculate the derivative term with a low-pass filter of first order
    double derivative = (error - previous_error_) / dt;
    derivative = previous_derivative_ + (dt/(RC + dt)) * (derivative - previous_derivative_); 

    if (!antiwindup_) {
    integral_ += error * dt;
    } 
    output = (!antiwindup_) ? (kp_ * error + ki_ * integral_ + kd_ * derivative) : (kp_ * error + kd_ * derivative);

    double clamped_output = clamping(output);
    bool antiwindup_ = antiwindup(output, clamped_output, error);

    // Update state variables for the next iteration
    previous_error_ = error;
    previous_time_ = current_time;
    previous_derivative_ = derivative;

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
    return (output > max_output_) ? max_output_ : ((output < min_output_) ? min_output_ : output);
}

bool PIDController::antiwindup(double output, double clamped_output, double error) {
    return (clamped_output != output) && (error * output > 0);
}

unsigned long int nanos() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long int)ts.tv_sec * 1000000000 + ts.tv_nsec;
}
