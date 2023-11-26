#include "integrator.h"
namespace mpcc{
Integrator::Integrator()
{
    std::cout << "default constructor, not everything is initialized properly" << std::endl;
}

Integrator::Integrator(double Ts,const PathToJson &path)
:model_(Ts,path)
{
}

State Integrator::RK4(const State &x, const Input &u,const double ts) const
{
    // 4th order Runge Kutta (RK4) implementation
    // 4 evaluation points of continuous dynamics
    const StateVector x_vec = stateToVector(x);
    const InputVector u_vec = inputToVector(u);
    // evaluating the 4 points
    const StateVector f1 = model_.getF(vectorToState(x_vec),u);
    const StateVector f2 = model_.getF(vectorToState(x_vec+ts/2.*k1),u);
    const StateVector f3 = model_.getF(vectorToState(x_vec+ts/2.*k2),u);
    const StateVector f4 = model_.getF(vectorToState(x_vec+ts*k3),u);
    // combining to give output
    const StateVector x_next = x_vec + ts*(f1/6.+f2/3.+f3/3.+f4/6.);
    return vectorToState(x_next);
}

State Integrator::EF(const State &x, const Input &u,const double ts) const
{
    // Euler Forward integration
    const StateVector x_vec = stateToVector(x);
    const StateVector f = model_.getF(x,u); // evaluation continuous dynmaics
    // compute next time step
    const StateVector x_next = x_vec + ts*f;
    return vectorToState(x_next);
}

State Integrator::simTimeStep(const State &x, const Input &u,const double ts) const
{
    // integrate time step
    State x_next = x;
    const int integration_steps = (int)(ts/fine_time_step_);
    if(ts/fine_time_step_ != integration_steps)
    {
        std::cout << "Warning" << std::endl;
    }
    for(int i = 0;i<integration_steps;i++)
        x_next = RK4(x_next,u,fine_time_step_);

    return x_next;
}
}