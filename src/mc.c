#include "mc.h"

int POTENTIAL_CUTTOFF = 3;

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

void mc_step(double* x, double* y, double* z, int n, double sigma, double T, int& counter, int power, domain_list_t& dl)
{
    int pidx;
    double enix_before = 0.0;
    double enix_after = 0.0;
    double x_old, y_old, z_old, dE;

    double acc, p;

    for (int i = 0; i < n; i++)
    {
        pidx = rand_int(0, n-1);
        enix_before = calc_atomic_rep_energy( x, y, z, n, pidx, sigma, power, dl);
        
        x_old = x[pidx];
        y_old = y[pidx];
        z_old = z[pidx];
        small_displacement(&x[pidx], &y[pidx], &z[pidx], sigma);
        enix_after = calc_atomic_rep_energy( x, y, z, n, pidx, sigma, power, dl);
        dE = enix_after - enix_before;

        acc = std::min(exp(-dE/T), 1.0 );
        p = uniform();
        if (p < acc)
        {
            counter++;
// #ifdef FAST
            dl.update_domain_for_node(x[pidx], y[pidx], z[pidx], pidx);
// #endif
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

double calc_rep_energy(double* x, double* y, double* z, int n, double sigma, int power, domain_list_t& dl)
{  
    double en = 0.0;
    for (int i = 0; i < n; i++)
    {
        en += 0.5 * calc_atomic_rep_energy( x, y, z, n, i, sigma, power, dl);
    }

    return en;
}

double calc_atomic_rep_energy(double* x, double* y, double* z, int n, int idx, double sigma, int power, domain_list_t& dl)
{
    int power_by_2 = power / 2;
        
    double en = 0.0;
    double eps = 1.0;
 
    double r2 = 0.0;
    double dx, dy, dz;
    double sigm_by_r_pow;
    double sigma2 = sigma*sigma;

    double rc = POTENTIAL_CUTTOFF*sigma;
    double rc2 = rc*rc;
    double Vc = pow( (sigma2 / rc2) , power_by_2);


//#ifdef FAST
    std::vector<int> my_neigs = dl.get_nb_lists(idx);
    int i;
    for (int j = 0; j < my_neigs.size(); j++)
    {
        i =  my_neigs[j];
// #else
//   for (int i = 0; i < n; i++)
//   {
// #endif
        if (i != idx )
        {
            dx = x[i] - x[idx];
            r2 = dx*dx;
            if (r2 >= rc2)
                continue;
            dy = y[i] - y[idx];
            r2 += dy*dy;
            if (r2 >= rc2)
                continue;
            dz = z[i] - z[idx];
            r2 += dz*dz;
            if (r2 >= rc2)
                continue;

            sigm_by_r_pow = pow( (sigma2 / r2) , power_by_2 ) - Vc;
            en += sigm_by_r_pow;
       }
    }
    
    en *= eps;
    return en;
}

double run_annealing(double* x, double* y, double* z, int n, int n_steps, int n_anneals, double Tmin, double Tmax, double sigma)
{
    double mult_T = log(Tmax / Tmin) / (n_anneals - 1);
    double T = Tmax;

    // Linked-list defs
    double EPS = 1e-6;
    double DL_SIGMA = POTENTIAL_CUTTOFF * sigma;
    int M = (1.0 - (-1.0) + 2*EPS) / DL_SIGMA;
    domain_list_t dl(M, false);
    dl.set_system_dims(-1-EPS, 1+EPS, 0);
    dl.set_system_dims(-1-EPS, 1+EPS, 1);
    dl.set_system_dims(-1-EPS, 1+EPS, 2);
    dl.get_nb_lists(x,y,z,n,DL_SIGMA); // init run
    //

    int acc = 0;

    for (int i = 0; i < n_anneals; i++)
    {  
        acc = 0;
        for(int j = 0; j < n_steps; j ++)
        {
            mc_step(x, y, z, n, sigma, T, acc, 8, dl);
        }

        T = Tmax * exp(-mult_T * i);
    }
    
    return calc_rep_energy(x, y, z, n, sigma, 8, dl);
}
