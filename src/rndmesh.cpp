#include "rmesh.h"

void generate_random_mesh(int n_, int n_steps_, int n_anneal_, double T_min_, double T_max_, double sigma_)
{
    int* n = new int;
    int* nrow = new int;
    *n = n_;
    *nrow = 6;
    
    //cout << "n="<<n<< endl;

    double* x_vec = new double[*n];
    double* y_vec = new double[*n];
    double* z_vec = new double[*n];

    int* list = new int[6*(*n-2)];
    int* lptr = new int[6*(*n-2)];
    int* lend = new int[*n];
    int* near = new int[*n];
    int* next = new int[*n];
    int* ltri = new int[*nrow*2*(*n-2)];
    double* dist = new double[*n];
    int* lnew = new int;
    int* ier = new int;
    int* nt = new int;


    //int n_anneal = 800;
    //double* x_anneal = new double[n_anneal];
    //double* y_anneal = new double[n_anneal];
    //double* z_anneal = new double[n_anneal];
    int n_steps = n_steps_;
    int n_anneals = n_anneal_;
    double Tmax = T_max_;
    double Tmin = T_min_;
    double sigma = sigma_;
    
    generate_n_random(n_, x_vec, y_vec, z_vec);
    run_annealing(x_vec, y_vec, z_vec, n_, n_steps, n_anneals, Tmin, Tmax, sigma);

//    for (int i = 0; i < n_anneal; i++)
//    {
//        cout << "H "<< x_anneal[i] << " " << y_anneal[i] << " " << z_anneal[i]<< "\n";
//    }

/*
    for (int i = 0; i < *n ; i++)
    {
       x_vec[i] = 2.0 * (double) std::rand() /  (double)RAND_MAX - 1.0;
       y_vec[i] = 2.0 * (double) std::rand() /  (double)RAND_MAX - 1.0;
       z_vec[i] = 2.0 * (double) std::rand() /  (double)RAND_MAX - 1.0;
       cout << "i="<< i <<" " << x_vec[i] << " " << y_vec[i] << " " << z_vec[i]<< "\n";
    }

    double r = 0.0;
    for (int i = 0; i < *n ; i++)
    {
       r = x_vec[i]*x_vec[i] + y_vec[i]*y_vec[i] + y_vec[i]*y_vec[i];
       r = sqrt(r);
       x_vec[i] /= r;
       y_vec[i] /= r;
       z_vec[i] /= r;
    } 

    for (int i = 0; i < *n ; i++)
    {
       cout << x_vec[i] << " " << y_vec[i] << " " << z_vec[i]<< "\n";
    }

*/

//    x_vec = x_anneal;
//    y_vec = y_anneal;
//    z_vec = z_anneal;

    trmesh_( n, x_vec, y_vec, z_vec, list, lptr, lend, lnew, near, next, dist, ier );
    trlist_( n, list, lptr, lend, nrow, nt, ltri, ier );

//    cout << "  Number of triangles = " << *nt << endl;
//    cout << "   " << endl;
//    cout << "    Check Euler''s formula:" << endl;
//    cout << "   " << endl;
//    cout << "    Faces =    " << *nt << endl;
//    cout << "    Vertices = " <<  *n << endl;
//    cout << "    Edges =    " << ( 3 * *nt ) / 2 << endl;
//    cout << "   " << endl;
//    cout << "    F+V-E-2 =  " <<  *nt + *n - ( ( 3 * (*nt) ) / 2 ) - 2 << endl;


//    cout << "PRINT TRIANGLES" << endl;
//    for (int i = 0; i < 2*(*n-2);i++)
//    {
//        cout <<  ltri[6*i + 0] << " " << ltri[6*i + 1] << " "<< ltri[6*i + 2] << "||" <<  ltri[6*i + 3] << " " << ltri[6*i + 4] << " "<< ltri[6*i + 5]  << endl;
//    }


//    cout << "ier=" << *ier << endl; 
//    cout << "OK"<< endl;
    delete n;

//*
//    cout << "OK"<< endl;
    delete[] x_vec;
    delete[] y_vec;
    delete[] z_vec;

//    cout << "OK"<< endl;
    delete[] list;
    delete[] lptr;
    delete[] lend;
    delete[] near;
    delete[] next;
    delete[] dist;

//    cout << "OK"<< endl;

    delete lnew;
    delete ier;
    delete nt;

//    delete[] x_anneal;
//    delete[] y_anneal;
//    delete[] z_anneal;

//    cout << "OK"<< endl;
    
    return;
}
