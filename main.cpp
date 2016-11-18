#include <stdlib.h>
#include <rndmesh.h>

int main(int argc, char** argv)
{

    srand (time(NULL));
    unsigned long init[4]={(unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand()}, length=4;
    init_by_array(init, length);
    
    int n_ = atoi (argv[1]);
    int n_steps = atoi (argv[2]);
    int n_anneals = atoi (argv[3]);
    double T_min = strtod( argv[4], NULL ) ;
    double T_max = strtod( argv[5], NULL );
    double scaled_sigma = strtod( argv[6], NULL );
   
    std::cerr << "n = " << n_ << std::endl;
    std::cerr << "n_steps = " << n_steps << std::endl;
    std::cerr << "n_anneals = " << n_anneals << std::endl;
    std::cerr << "T_min = " << T_min << std::endl;
    std::cerr << "T_max = " << T_max << std::endl;
    std::cerr << "scaled_sigma = " << scaled_sigma << std::endl;
    
    int nrow = 6;
    double* xyz = new double[3 * n_];
    int* ltri = new int[nrow * 2 * (n_ - 2)];
   

    double energy = generate_random_points(n_, xyz, n_steps, n_anneals, T_min, T_max, scaled_sigma);
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

    std::cout << n_ << " " << energy << std::endl;
    for (int i = 0; i < n_; i++)
    {
        std::cout << x_vec[i] << " " << y_vec[i] << " " << z_vec[i] << std::endl;
    }

    std::cout << std::endl;
    for (int i = 0; i < 2*(n_ - 2); i++)
    {
        std::cout << ltri[6*i + 0] << " " << ltri[6*i + 1] << " "<< ltri[6*i + 2] << "||";
        std::cout << ltri[6*i + 3] << " " << ltri[6*i + 4] << " "<< ltri[6*i + 5] << std::endl;
    }

    delete[] xyz;
    delete[] ltri;
    
    delete[] x_vec;
    delete[] y_vec;
    delete[] z_vec;

    return 0;
}
