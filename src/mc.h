#ifndef MC_H
#define MC_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include "random.h"

#ifdef __cplusplus
extern "C" {
#endif

void random_rotation(double* x, double*, double* z);
void generate_n_random(int n, double* x, double* y, double* z);
double mc_step(double* x, double* y, double* z, int n, double sigma, double T, int& counter, int power);

double calc_rep_energy(double* x, double* y, double* z, int n, double sigm, int power);
double calc_atomic_rep_energy(double* x, double* y, double* z, int n, int idx, double sigma, int power);

void run_annealing(double* x, double* y, double* z, int n, int n_steps, int n_anneals, double Tmin, double Tmax, double sigma);


#ifdef  __cplusplus
}
#endif

#endif  /* MC_H */
