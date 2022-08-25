#ifndef FHN_H_
#define FHN_H_

#define NUMBER_EQUATIONS_CELL_MODEL 2

#include <cmath>

// Precision to be used for the calculations
typedef double real;

void setIC_ode_cpu(real *sv);
void solve_ode_cpu(real dt, real *sv, real Istim);
void solve_Forward_Euler_cpu(real *sv, real dt, real Istim);
void RHS_fhn_cpu(real *rY, real *rDY, real Istim);


#endif /* FHN_H_ */
