#include <iostream>

/*
    PID Controller
    
    TODO: 

        1. Implement anti-windup on integral term -- FATTOOO piu o meno
        2. Implement derivative on measurement
        3. Implemete low pass filter on derivative term  --> IL TAU sarà due volte la frequenza di campionamento
*/

class PIDController {
public:
    PIDController(double kp, double ki, double kd) : kp_(kp), ki_(ki), kd_(kd), integral_(0), previous_error_(0), antiwindup_(false) {}

    double Calculate(double setpoint, double current_value) {
        double error = setpoint - current_value; // vel desiderata - vel attuale
        double output = 0.0;
        double derivative = (error - previous_error_);
        // derivative = lastDerivative + (timeDelta / (timeDelta + RC)) * (derivative - lastDerivative);

        if (antiwindup_ == false) {
            std::cerr <<"antiwindup false"<<std::endl;
            integral_ += error;
            double output = kp_*error + ki_ * integral_ + kd_ * derivative;
        } 
        else {
            std::cerr <<"antiwindup true"<<std::endl;
            double output = kp_ * error + kd_ * derivative;
            antiwindup_ = false;
        }
        
        double clamped_output = clamping(output);
        bool antiwindup_ = antiwindup(output, clamped_output, error);
        previous_error_ = error;

        return clamped_output;
    }

    double clamping(double output) {
        if (output > 100) {
            output = 100;
        }
        else if (output < -100) {
            output = -100;
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
    double integral_;
    double previous_error_;
    // double lastDerivative_;
    bool antiwindup_;
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
