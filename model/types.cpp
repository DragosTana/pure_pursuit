#include "types.h"
namespace mpcc{

StateVector stateToVector(const State &x)
{
    StateVector xk;
    xk(0) = x.X;
    xk(1) = x.Y;
    xk(2) = x.phi;
    xk(3) = x.vx;
    xk(4) = x.vy;
    xk(5) = x.r;
    xk(6) = x.s;
    xk(7) = x.D;
    xk(8) = x.delta;
    xk(9) = x.vs;
    return xk;
}

InputVector inputToVector(const Input &u)
{
    InputVector uk = {u.dD,u.dDelta,u.dVs};
    return uk;
}

State vectorToState(const StateVector &xk)
{
    State x;
    x.X     = xk(0);
    x.Y     = xk(1);
    x.phi   = xk(2);
    x.vx    = xk(3);
    x.vy    = xk(4);
    x.r     = xk(5);
    x.s     = xk(6);
    x.D     = xk(7);
    x.delta = xk(8);
    x.vs    = xk(9);

    return x;
}

Input vectorToInput(const InputVector &uk)
{
    Input u;
    u.dD     = uk(0);
    u.dDelta = uk(1);
    u.dVs    = uk(2);

    return u;
}

State arrayToState(double *xk)
{
    State x;
    x.X     = xk[0];
    x.Y     = xk[1];
    x.phi   = xk[2];
    x.vx    = xk[3];
    x.vy    = xk[4];
    x.r     = xk[5];
    x.s     = xk[6];
    x.D     = xk[7];
    x.delta = xk[8];
    x.vs    = xk[9];

    return x;
}

Input arrayToInput(double *uk)
{
    Input u;
    u.dD     = uk[0];
    u.dDelta = uk[1];
    u.dVs    = uk[2];

    return u;
}

}