#ifndef RMESH_H
#define RMESH_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "mc.h"


#ifdef __cplusplus
extern "C" {
#endif
// STRIPACK LIBRARY
void trmesh_(int *n, double *x, double *y, double *z, int *list, int *lptr, int *lend, int *lnew, int *near, int *next, double *dist, int *ier );
void trlist_(int *n, int *list, int *lptr, int *lend, int* nrow, int* nt, int* ltri, int* ier );
#ifdef __cplusplus
}
#endif

double generate_random_mesh(int n_, int n_steps_, int n_anneal_, double T_min_, double T_max_, double sigma_);
double generate_random_points(int n_, double* xyz, int n_steps_, int n_anneal_, double T_min_, double T_max_, double sigma_);
void traingulate_points(int n_, double* xyz, int* ltri);


#endif  /* RMESH_H */
