#include "mc.h"

int rand_int(int a, int b) 
{
   return (int)( (b - a + 1) * uniform() ) + a;
}

void small_displacement(double* x, double* y, double* z, double sigma)
{
   double del_ = 0.25 * sigma;
   *x += uniform(-del_, del_);
   *y += uniform(-del_, del_);
   *z += uniform(-del_, del_);

   double dx_2, dy_2, dz_2, r;
   dx_2 = (*x)*(*x);
   dy_2 = (*y)*(*y);
   dz_2 = (*z)*(*z);
   
   r = sqrt(dx_2 + dy_2 +  dz_2);
   *x /= r;
   *y /= r;
   *z /= r;
   return;
}

void random_rotation(double* x, double* y, double* z)
{
    double xi = 1.0;
    double yi = 0.0;
    double zi = 0.0;

    double xnew, ynew, znew;    

    double u1 = uniform();
    double u2 = uniform();
    double u3 = uniform();
    double q0 = sqrt(1.0 - u1) * sin(2.0 * M_PI * u2);
    double q1 = sqrt(1.0 - u1) * cos(2.0 * M_PI * u2);
    double q2 = sqrt(u1) * sin(2.0 * M_PI * u3);
    double q3 = sqrt(u1) * cos(2.0 * M_PI * u3);
    double A[3][3];
    A[0][0] = q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3;
    A[0][1] = 2 * (q1 * q2 + q0 * q3);
    A[0][2] = 2 * (q1 * q3 - q0 * q2);
    A[1][0] = 2 * (q1 * q2 - q0 * q3);
    A[1][1] = q0 * q0 - q1 * q1 + q2 * q2 - q3 * q3;
    A[1][2] = 2 * (q2 * q3 + q0 * q1);
    A[2][0] = 2 * (q1 * q3 + q0 * q2);
    A[2][1] = 2 * (q2 * q3 - q0 * q1);
    A[2][2] = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    xnew  = A[0][0] * xi;
    xnew += A[0][1] * yi;
    xnew += A[0][2] * zi;
    ynew  = A[1][0] * xi;
    ynew += A[1][1] * yi;
    ynew += A[1][2] * zi;
    znew  = A[2][0] * xi;
    znew += A[2][1] * yi;
    znew += A[2][2] * zi;
    *x = xnew;
    *y = ynew;
    *z = znew;
    return;
}

void generate_n_random(int n, double* xv, double* yv, double* zv)
{    
    for (int i = 0; i < n; i++)
    {
         random_rotation(&xv[i], &yv[i], &zv[i]);
    }
    return;
}

void mc_step(double* x, double* y, double* z, int n, double sigma, double T, int& counter, int power)
{
    int pidx = -1;
    double enix_before = 0.0;
    double enix_after = 0.0;
    double dE;
    double x_old, y_old, z_old;

    double acc, p;

    for (int i = 0; i < n; i++)
    {
        pidx = rand_int(0, n-1);
        enix_before = calc_atomic_rep_energy( x, y, z, n, pidx, sigma, power);
        
        x_old = x[pidx];
        y_old = y[pidx];
        z_old = z[pidx];
//        random_rotation(&x[pidx], &y[pidx], &z[pidx]);
        small_displacement(&x[pidx], &y[pidx], &z[pidx], sigma);
        enix_after = calc_atomic_rep_energy( x, y, z, n, pidx, sigma, power);
        dE = enix_after - enix_before;

        acc = std::min(exp(-dE/T), 1.0 );
        p = uniform();
        if (p < acc)
        {
            counter++;
        }
        else
        {
            x[pidx] = x_old;
            y[pidx] = y_old;
            z[pidx] = z_old;
        }
    }

    return;
}

double calc_rep_energy(double* x, double* y, double* z, int n, double sigma, int power)
{
   double en = 0.0;
   double eps = 1.0;
 
   double r = 0.0;
   double dx, dy, dz;
   double sigm_by_r12;   

   for (int i = 0; i < n; i++)
   {
       for (int j = i + 1; j < n; j++)
       {
             dx = x[i] - x[j];
             dy = y[i] - y[j];
             dz = z[i] - z[j];
             r = dx*dx + dy*dy + dz*dz;
             r = fastmath::fast_sqrt(r);
             sigm_by_r12 = pow( (sigma/r) , power );
             en += eps * sigm_by_r12;
       }
   }

   return en;
}

double calc_atomic_rep_energy(double* x, double* y, double* z, int n, int idx, double sigma, int power)
{
   double en = 0.0;
   double eps = 1.0;
 
   double r = 0.0;
   double dx, dy, dz;
   double sigm_by_r12;   

   for (int i = 0; i < n; i++)
   {
      if (i != idx )
      {
          dx = x[i] - x[idx];
          dy = y[i] - y[idx];
          dz = z[i] - z[idx];
          r = dx*dx + dy*dy + dz*dz;
          r = fastmath::fast_sqrt(r);
          sigm_by_r12 = pow( (sigma/r) , power );
          en += eps * sigm_by_r12;
      }
   }

   return en;
}

double run_annealing(double* x, double* y, double* z, int n, int n_steps, int n_anneals, double Tmin, double Tmax, double sigma)
{
    double mult_T = log(Tmax / Tmin) / (n_anneals - 1);
    double T = Tmax;
 
    int acc = 0;

    for(int power_index = 1; power_index < 12; power_index++)
    {
        mc_step(x, y, z, n, sigma, T, acc, power_index);
    }

    for (int i = 0; i < n_anneals; i++)
    {  
        acc = 0;

        for(int j = 0; j < n_steps; j ++)
        {
            mc_step(x, y, z, n, sigma, T, acc, 6); //12);
        }

        T = Tmax * exp(-mult_T * i);
    }
    
    double energy = calc_rep_energy(x, y, z, n, sigma, 6); //12);
    return energy;
}
