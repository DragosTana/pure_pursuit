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
#define DEBUG

class PIDController {
public:
    PIDController(double kp, double ki, double kd, double min_output , double max_output) : 
        kp_(kp), 
        ki_(ki), 
        kd_(kd), 
        min_output_(min_output),
        max_output_(max_output),
        integral_(0), 
        previous_error_(0), 
        antiwindup_(false) {}

    double Calculate(double setpoint, double current_value) {
        double error = setpoint - current_value; // vel desiderata - vel attuale
        double output = 0.0;
        double derivative = (error - previous_error_);
        // derivative = lastDerivative + (timeDelta / (timeDelta + RC)) * (derivative - lastDerivative);

        if (antiwindup_ == false) {
            integral_ += error;
            output = kp_*error + ki_ * integral_ + kd_ * derivative;
        } 
        else {
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

    double clamping(double output) {
        if (output > max_output_) {
            output = max_output_;
        }
        else if (output < min_output_) {
            output = min_output_;
        }
        return output;
    }

    bool antiwindup(double output, double clamped_output, double error) {

        if ((clamped_output != output) && (error * output > 0)) {
            return true;
        }
        return false;
    }

private:
    double kp_;
    double ki_;
    double kd_;
    double min_output_;
    double max_output_;
    double integral_;
    double previous_error_;
    // double lastDerivative_;
    bool antiwindup_;
};

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
