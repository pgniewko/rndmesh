#ifndef MC_H
#define MC_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include "random.h"

#ifdef FAST
#include <domain_list_t.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void random_rotation(double* x, double* y, double* z);
void small_displacement(double* x, double* y, double* z, double sigma, double rx, double ry, double rz);
void generate_n_random(int n, double* xyz, double rx, double ry, double rz);
double run_annealing(double* xyz, int n, int n_steps, int n_anneals, double Tmin, double Tmax, double sigma, double rx, double ry, double rz);

#ifdef FAST
void mc_step(double* xyz, int n, double sigma, double T, int& counter, int power, domain_list_t& dl, double rx, double ry, double rz);
double calc_rep_energy(double* xyz, int n, double sigm, int power, domain_list_t& dl);
double calc_atomic_rep_energy(double* xyz, int n, int idx, double sigma, int power, domain_list_t& dl);
#else
void mc_step(double* xyz, int n, double sigma, double T, int& counter, int power, double rx, double ry, double rz);
double calc_rep_energy(double* xyz, int n, double sigm, int power);
double calc_atomic_rep_energy(double* xyz, int n, int idx, double sigma, int power);
#endif

#ifdef  __cplusplus
}
#endif

#endif  /* MC_H */
