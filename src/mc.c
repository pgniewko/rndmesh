#include "mc.h"

int POTENTIAL_CUTTOFF = 3;

int rand_int(int a, int b) 
{
   return (int)( (b - a + 1) * uniform() ) + a;
}

void small_displacement(double* x, double* y, double* z, double sigma, double rx, double ry, double rz)
{
    *x /= rx;
    *y /= ry;
    *z /= rz;

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

    *x *= rx;
    *y *= ry;
    *z *= rz;

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

void generate_n_random(int n, double* xyz, double rx, double ry, double rz)
{    
    for (int i = 0; i < n; i++)
    {
         random_rotation(&xyz[3*i+0], &xyz[3*i+1], &xyz[3*i+2]);
    }

    // put on the ellipsoid
    for (int i = 0; i < n; i++)
    {
        xyz[3*i + 0] *= rx;
        xyz[3*i + 1] *= ry;
        xyz[3*i + 2] *= rz;
    }
    return;
}

#ifdef FAST
void mc_step(double* xyz, int n, double sigma, double T, int& counter, int power, domain_list_t& dl, double rx, double ry, double rz)
#else
void mc_step(double* xyz, int n, double sigma, double T, int& counter, int power, double rx, double ry, double rz)
#endif
{
    int pidx;
    double enix_before = 0.0;
    double enix_after = 0.0;
    double x_old, y_old, z_old, dE;

    double acc, p;

    for (int i = 0; i < n; i++)
    {
        pidx = rand_int(0, n-1);
#ifdef FAST
        enix_before = calc_atomic_rep_energy(xyz, n, pidx, sigma, power, dl);
#else
        enix_before = calc_atomic_rep_energy(xyz, n, pidx, sigma, power);
#endif

        x_old = xyz[3 * pidx + 0];
        y_old = xyz[3 * pidx + 1];
        z_old = xyz[3 * pidx + 2];
        small_displacement(&xyz[3 * pidx + 0], &xyz[3 * pidx + 1], &xyz[3 * pidx + 2], sigma, rx, ry, rz);
#ifdef FAST
        enix_after = calc_atomic_rep_energy(xyz, n, pidx, sigma, power, dl);
#else
        enix_after = calc_atomic_rep_energy(xyz, n, pidx, sigma, power);
#endif
        dE = enix_after - enix_before;

        acc = std::min(exp(-dE/T), 1.0 );
        p = uniform();
        if (p < acc)
        {
            counter++;
#ifdef FAST
            // UPDATE LINKED-DOMAINS UPON MOVE ACCEPTANCE
            dl.update_domain_for_node(xyz[3 * pidx + 0], xyz[3 * pidx + 1], xyz[3 * pidx + 2], pidx);
#endif
        }
        else
        {
            xyz[3 * pidx + 0] = x_old;
            xyz[3 * pidx + 1] = y_old;
            xyz[3 * pidx + 2] = z_old;
        }
    }

    return;
}

#ifdef FAST
double calc_rep_energy(double* xyz, int n, double sigma, int power, domain_list_t& dl)
#else
double calc_rep_energy(double* xyz, int n, double sigma, int power)
#endif
{  
    double en = 0.0;
    for (int i = 0; i < n; i++)
    {
#ifdef FAST 
        en += 0.5 * calc_atomic_rep_energy(xyz, n, i, sigma, power, dl);
#else
        en += 0.5 * calc_atomic_rep_energy(xyz, n, i, sigma, power);
#endif
    }

    return en;
}

#ifdef FAST
double calc_atomic_rep_energy(double* xyz, int n, int idx, double sigma, int power, domain_list_t& dl)
#else
double calc_atomic_rep_energy(double* xyz, int n, int idx, double sigma, int power)
#endif
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


#ifdef FAST
    std::vector<int> my_neigs = dl.get_nb_lists(idx);
    int i;
    for (int j = 0; j < my_neigs.size(); j++)
    {
        i =  my_neigs[j];
#else
    for (int i = 0; i < n; i++)
    {
#endif
        if (i != idx)
        {
            dx = xyz[3 * i + 0] - xyz[3 * idx + 0];
            r2 = dx * dx;
            if (r2 >= rc2)
                continue;
            dy = xyz[3 * i + 1] - xyz[3 * idx + 1];
            r2 += dy * dy;
            if (r2 >= rc2)
                continue;
            dz = xyz[3 * i + 2] - xyz[3 * idx + 2];
            r2 += dz * dz;
            if (r2 >= rc2)
                continue;

            sigm_by_r_pow = pow((sigma2 / r2), power_by_2) - Vc;
            en += sigm_by_r_pow;
       }
    }
    
    en *= eps;
    return en;
}


double run_annealing(double* xyz, int n, int n_steps, int n_anneals, double Tmin, double Tmax, double sigma, double rx, double ry, double rz)
{
    double mult_T = log(Tmax / Tmin) / (n_anneals - 1);
    double T = Tmax;

#ifdef FAST
    // Linked-list defs
    double EPS = 1e-6;

    double max_size =  std::max(rx, ry);
    max_size = std::max(max_size, rz);

    double DL_SIGMA = POTENTIAL_CUTTOFF * sigma;
    int M = (1.0 - (-1.0) + 2*EPS) / DL_SIGMA;
    domain_list_t dl(M, false);
    dl.set_system_dims(-max_size-EPS, max_size+EPS, 0);
    dl.set_system_dims(-max_size-EPS, max_size+EPS, 1);
    dl.set_system_dims(-max_size-EPS, max_size+EPS, 2);
    dbl_vec xyz_wrapper;
    for (int i = 0; i < n; i++)
    {
        xyz_wrapper.push_back(xyz[3 * i + 0]);
        xyz_wrapper.push_back(xyz[3 * i + 1]);
        xyz_wrapper.push_back(xyz[3 * i + 2]);
    }
    dl.get_nb_lists(xyz_wrapper, n, DL_SIGMA);
#endif

    int acc = 0;

    for (int i = 0; i < n_anneals; i++)
    {
        acc = 0;
        for(int j = 0; j < n_steps; j ++)
        {
#ifdef FAST
            mc_step(xyz, n, sigma, T, acc, 8, dl, rx, ry, rz);
#else
            mc_step(xyz, n, sigma, T, acc, 8, rx, ry, rz);
#endif
        }

        T = Tmax * exp(-mult_T * i);
    }
   
#ifdef FAST
    return calc_rep_energy(xyz, n, sigma, 8, dl);
#else
    return calc_rep_energy(xyz, n, sigma, 8);
#endif
}
