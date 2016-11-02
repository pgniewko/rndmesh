#include "mc.h"

int rand_int(int a, int b) 
{
   return (int)( (b - a + 1) * uniform() ) + a;
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
}

void generate_n_random(int n, double* xv, double* yv, double* zv)
{    
    for (int i = 0; i < n; i++)
    {
         random_rotation(&xv[i], &yv[i], &zv[i]);
    }
}

double mc_step(double* x, double* y, double* z, int n, double sigma, double T, int& counter, int power)
{
    double energy = 0.0;
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
        random_rotation(&x[pidx], &y[pidx], &z[pidx]);
        enix_after = calc_atomic_rep_energy( x, y, z, n, pidx, sigma, power);
        dE = enix_after - enix_before;

        acc = std::min(exp(-dE/T), 1.0 );
        p = uniform();
        if (p < acc)
        {
            counter++;
 //           energy += dE;
        }
        else
        {
            x[pidx] = x_old;
            y[pidx] = y_old;
            z[pidx] = z_old;
        }
    }

//    std::cerr << "E = " << energy << std::endl;
    return energy;
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
             r = sqrt(r);
//             if (r < 0.001)
//             {
//                  r = 0.001;
//             }
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
          r = sqrt(r);
//          if (r < 0.001)
//          {
//              r = 0.001;
//          }
          sigm_by_r12 = pow( (sigma/r) , power );
          en += eps * sigm_by_r12;
      }
   }

   return en;
}

void run_annealing(double* x, double* y, double* z, int n, int n_steps, int n_anneals, double Tmin, double Tmax, double sigma)
{
    double mult_T = log(Tmax / Tmin) / (n_anneals - 1);
    double T = Tmax;
 
    int acc = 0;
    double energy, energy_check;


    for(int power_index = 1; power_index < 12; power_index++)
    {
        energy = mc_step(x, y, z, n, sigma, T, acc, power_index);
    }

    for (int i = 0; i < n_anneals; i++)
    {  
        acc = 0;

        for(int j = 0; j < n_steps; j ++)
        {
            energy = mc_step(x, y, z, n, sigma, T, acc, 12);
        }
        
        energy = calc_rep_energy( x,  y, z, n, sigma, 12);
        std::cerr << "Anneal step: " << (i+1) << " out of:" << n_anneals << " energy = " << energy << std::endl;
        
         
        std::cout << n << "\n";
        for (int ix = 0; ix < n; ix++)
        {
             std::cout << "H "<< x[ix] << " " << y[ix] << " " << z[ix]<< "\n";
        }

        T = Tmax * exp(-mult_T * i);
    }
}
