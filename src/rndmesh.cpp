#include "rndmesh.h"


double generate_random_points(int n_, double* xyz, int n_steps_, int n_anneal_, double T_min_, double T_max_, double sigma_)
{
    double energy;

    int* n = new int;
    *n = n_;

    double* x_vec = new double[*n];
    double* y_vec = new double[*n];
    double* z_vec = new double[*n];

    int n_steps = n_steps_;
    int n_anneals = n_anneal_;
    double Tmax = T_max_;
    double Tmin = T_min_;
    double sigma = sigma_;
    
    generate_n_random(n_, x_vec, y_vec, z_vec);
    energy = run_annealing(x_vec, y_vec, z_vec, n_, n_steps, n_anneals, Tmin, Tmax, sigma);

    for (int i = 0; i < n_; i++)
    {
        xyz[3*i + 0] = x_vec[i];
        xyz[3*i + 1] = y_vec[i];
        xyz[3*i + 2] = z_vec[i];
    }

    delete n;

    delete[] x_vec;
    delete[] y_vec;
    delete[] z_vec;

    return energy;
}


void traingulate_points(int n_, double* xyz, int* ltri)
{

    int* n = new int;
    int* nrow = new int;
    *n = n_;
    *nrow = 6;

    double* x_vec = new double[*n];
    double* y_vec = new double[*n];
    double* z_vec = new double[*n];

    for (int i = 0; i < n_; i++)
    {
        x_vec[i] = xyz[3 * i + 0];
        y_vec[i] = xyz[3 * i + 1];
        z_vec[i] = xyz[3 * i + 2];
    }

    int* list = new int[6*(*n-2)];
    int* lptr = new int[6*(*n-2)];
    int* lend = new int[*n];
    int* near = new int[*n];
    int* next = new int[*n];
    double* dist = new double[*n];
    int* lnew = new int;
    int* ier = new int;
    int* nt = new int;

    trmesh_( n, x_vec, y_vec, z_vec, list, lptr, lend, lnew, near, next, dist, ier );
    trlist_( n, list, lptr, lend, nrow, nt, ltri, ier );

    // IF THE ERROR IS DETECTED THEN PRINT OUT THIS MESSAGE
    if ( *nt + *n - ( ( 3 * (*nt) ) / 2 ) - 2 != 0)
    {
        std::cout << "  ERROR DETECTED !" << std::endl;
        std::cout << "  Number of triangles = " << *nt << std::endl;
        std::cout << "   " << std::endl;
        std::cout << "    Check Euler''s formula:" << std::endl;
        std::cout << "   " << std::endl;
        std::cout << "    Faces =    " << *nt << std::endl;
        std::cout << "    Vertices = " <<  *n << std::endl;
        std::cout << "    Edges =    " << ( 3 * *nt ) / 2 << std::endl;
        std::cout << "   " << std::endl;
        std::cout << "    F+V-E-2 =  " <<  *nt + *n - ( ( 3 * (*nt) ) / 2 ) - 2 << std::endl;
    }


    delete n;
    delete nrow;

    delete[] x_vec;
    delete[] y_vec;
    delete[] z_vec;

    delete[] list;
    delete[] lptr;
    delete[] lend;
    delete[] near;
    delete[] next;
    delete[] dist;

    delete lnew;
    delete ier;
    delete nt;

    return;
}


double generate_random_mesh(int n_, int n_steps_, int n_anneal_, double T_min_, double T_max_, double sigma_)
{
    double energy;

    int nrow = 6;
    double* xyz = new double[3 * n_];
    int* ltri = new int[nrow * 2 * (n_ - 2)];
    
    energy = generate_random_points(n_, xyz, n_steps_, n_anneal_, T_min_, T_max_, sigma_);    
    traingulate_points(n_, xyz, ltri);

    double* x_vec = new double[n_];
    double* y_vec = new double[n_];
    double* z_vec = new double[n_];

    for (int i = 0; i < n_; i++)
    {
        x_vec[i] = xyz[3 * i + 0];
        y_vec[i] = xyz[3 * i + 1];
        z_vec[i] = xyz[3 * i + 2];
    }
    
    for (int i = 0; i < n_; i++)
    {
        std::cout << "H "<< x_vec[i] << " " << y_vec[i] << " " << z_vec[i] << "\n";
    }

    std::cout << std::endl;
    for (int i = 0; i < 2*(n_ - 2); i++)
    {
        std::cout << ltri[6*i + 0] << " " << ltri[6*i + 1] << " "<< ltri[6*i + 2] << "||";
        std::cout << ltri[6*i + 3] << " " << ltri[6*i + 4] << " "<< ltri[6*i + 5] << std::endl;
    }

    delete[] xyz;
    delete[] x_vec;
    delete[] y_vec;
    delete[] z_vec;

    delete[] ltri;

    return energy;
}

double calcQl(double *x, double *y, double *z, int l, double r)
{
    return 0.0;
}


double calcWl(double *x, double *y, double *z, int l, double r)
{
    return 0.0;
}

