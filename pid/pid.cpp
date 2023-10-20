#include <iostream>

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
    double kp = 0.1;
    double ki = 0.01;
    double kd = 0.05;

    PIDController pid_controller(kp, ki, kd);

    double setpoint = 100.0;
    double current_value = 50.0;

    for (int i = 0; i < 100; ++i) {
        double output = pid_controller.Calculate(setpoint, current_value);
        current_value += output;
        std::cout << "Iteration " << i << ": Current Value = " << current_value << std::endl;
    }

    return 0;
}
