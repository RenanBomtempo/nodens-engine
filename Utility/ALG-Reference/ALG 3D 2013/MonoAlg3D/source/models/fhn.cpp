#include "fhn.h"

// Setting initial conditions to the FitzHugh-Nagumo model
void setIC_ode_cpu(real *sv)
{
	// Vector containing the initial conditions of the cell
	int NEQ = NUMBER_EQUATIONS_CELL_MODEL;
	real IC[NEQ];

	// V; Units: millivolt; Initial value: -83.853
	IC[0] = -1.19940803524;
	// m; Units: dimensionless; Initial value: 0.00187018
	IC[1] = -0.624260044055;

	for(int i = 0; i < NEQ; i++)
		sv[i] = IC[i];
}

// Solving the Luo-Rudy 1991 model for each cell in the tissue matrix ni x nj
void solve_ode_cpu(real dt, real *sv, real Istim)
{
	solve_Forward_Euler_cpu(sv, dt, Istim);
}

// The Forward Euler Method
void solve_Forward_Euler_cpu(real *sv, real dt, real Istim)
{

	int NEQ = NUMBER_EQUATIONS_CELL_MODEL;

	real rY[NEQ], rDY[NEQ];

	for(int i = 0; i < NEQ; i++)
		rY[i] = sv[i];

	RHS_fhn_cpu(rY, rDY, Istim);

	for(int i = 0; i < NEQ; i++)
		sv[i] = dt*rDY[i] + rY[i];
}

void RHS_fhn_cpu(real *rY, real *rDY, real Istim)
{
	const real eps   = 0.2;
	const real gamma = 0.8;
	const real beta  = 0.7;

	real v = rY[0] + Istim;
	real w = rY[1];
	real v3;

	v3 = pow(v,3);

	real d_dt_v = ((1.0/eps) * (v-((1.0/3.0)*v3)-w));
	real d_dt_w = eps * (v - gamma*w + beta);

	rDY[0] = d_dt_v;
	rDY[1] = d_dt_w;
}

