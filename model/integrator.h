#ifndef MPCC_INTEGRATOR_H
#define MPCC_INTEGRATOR_H

#include "config.h"
#include "model.h"
#include "types.h"

namespace mpcc{
class Integrator {
public:
    State RK4(const State &x, const Input &u,double ts) const;
    State EF(const State &x, const Input &u,double ts) const;
    State simTimeStep(const State &x, const Input &u,double ts) const;

    Integrator();
    Integrator(double Ts, const PathToJson &path);

private:
    const double fine_time_step_ = 0.001;

    Model model_;
};
}
#endif //MPCC_INTEGRATOR_H