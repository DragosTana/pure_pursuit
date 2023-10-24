#include <iostream>

/*
    PID Controller
    
    TODO: 

        1. Implement anti-windup on integral term
        2. Implement derivative on measurement
*/

class PIDController {
public:
    PIDController(double kp, double ki, double kd) : kp_(kp), ki_(ki), kd_(kd), integral_(0), previous_error_(0) {}

    double Calculate(double setpoint, double current_value) {
        double error = setpoint - current_value;
        integral_ += error;
        double derivative = error - previous_error_;
        double output = kp_ * error + ki_ * integral_ + kd_ * derivative;
        previous_error_ = error;
        return output;
    }

private:
    double kp_;
    double ki_;
    double kd_;
    double integral_;
    double previous_error_;
};

int main() {
    // TO BE TUNED
    double kp = 0.5;
    double ki = 0;
    double kd = 0;

    PIDController pid_controller(kp, ki, kd);

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
