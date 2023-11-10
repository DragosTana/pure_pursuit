#pragma once

class PIDController {
public:
    PIDController(double kp, double ki, double kd, double min_output, double max_output);

    double Calculate(double setpoint, double current_value);

private:
    double kp_;
    double ki_;
    double kd_;
    double min_output_;
    double max_output_;
    double integral_;
    double previous_error_;
    bool antiwindup_;

    double clamping(double output);

    bool antiwindup(double output, double clamped_output, double error);
};
