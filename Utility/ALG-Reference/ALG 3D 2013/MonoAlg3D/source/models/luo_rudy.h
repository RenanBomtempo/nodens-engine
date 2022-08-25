/*
 * Luo_Rudy_1991.h
 *
 *  Created on: 19/05/2011
 *      Author: sachetto
 */

#ifndef LUO_RUDY_1991_H_
#define LUO_RUDY_1991_H_

#define NUMBER_EQUATIONS_CELL_MODEL 8

#include <cmath>

// Precision to be used for the calculations
typedef double real;

void setIC_ode_cpu(real *sv);
void solve_ode_cpu(real dt, real *sv, real Istim);
void solve_Forward_Euler_cpu(real *sv, real dt, real Istim);
void RHS_Luo_Rudy_1991_cpu(real *rY, real *rDY, real Istim);

#endif /* LUO_RUDY_1991_H_ */
