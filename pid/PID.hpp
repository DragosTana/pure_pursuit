#ifndef PID_HPP
#define PID_HPP


unsigned long int nanos();

/*  
*   PID Controller
*   @param kp Proportional gain
*   @param ki Integral gain
*   @param kd Derivative gain
*   @param min_output Minimum output value
*   @param max_output Maximum output value
*   @param low_pass_cutoff_hz Low pass filter cutoff frequency
*   @return PIDController object
*/
class PIDController {
public:
    PIDController(double kp, double ki, double kd, double min_output, double max_output, double low_pass_cutoff_hz );

    /*
    *   Calculate the PID output
    *   @param setpoint The desired value
    *   @param current_value The current value
    *   @return The PID output
    */
    double calculate(double setpoint, double current_value);

private:
    double kp_;
    double ki_;
    double kd_;
    double min_output_;
    double max_output_;
    double low_pass_cutoff_hz_;
    double integral_;
    double previous_error_;
    unsigned long int previous_time_;
    double previous_derivative_;
    bool antiwindup_;

    /*
    *   Clamping the output value.
    *   Limit the 'output' value to be within the specified range [min_output_, max_output_].
    *   @param output The output value
    *   @return The clamped output value
    */
    double clamping(double output);

    /*
    *   Antiwindup occurs if the clamped output is different from the unclamped output
    *   if the error and the output have the same direction. This prevents the integrator
    *   from accumulating error when the control action saturates.
    * 
    *   Check if the clamped output is different from the unclamped output
    *   and if the error and output have the same direction.
    * 
    *   @param output The output value
    *   @param clamped_output The clamped output value
    *   @param error The error value
    *   @return Whether antiwindup is triggered
    */
    bool antiwindup(double output, double clamped_output, double error);
};


#endif // PID_HPP