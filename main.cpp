#include <stdlib.h>
#include <rndmesh.h>
#include <ctime>

int main(int argc, char** argv)
{
    if (argc != 7)
    {
        std::cerr << "THE CODE REQUIRES 7 ARGUMENTS"<< std::endl;
        return 0;
    }
#ifdef FAST
    std::cout << "RUNNING PERFORMANCE OPTIMIZED CODE"<< std::endl;
#else
    std::cout << "RUNNING >>NOT<< PERFORMANCE OPTIMIZED CODE"<< std::endl;
#endif
    srand (time(NULL));
    unsigned long init[4]={(unsigned long)123, (unsigned long)234, (unsigned long)345, (unsigned long)456}, length=4;
    init_by_array(init, length);
    
    int n_ = atoi (argv[1]);
    int n_steps = atoi (argv[2]);
    int n_anneals = atoi (argv[3]);
    double T_min = strtod( argv[4], NULL ) ;
    double T_max = strtod( argv[5], NULL );
    double scaled_sigma = strtod( argv[6], NULL );

    int nrow = 6;
    double* xyz = new double[3 * n_];
    int* ltri = new int[nrow * 2 * (n_ - 2)];
    
    double energy;

    std::cerr << "n= "             << n_           << " ";// << std::endl;
    std::cerr << "n_steps= "       << n_steps      << " ";// << std::endl;
    std::cerr << "n_anneals= "     << n_anneals    << " ";// << std::endl;
    std::cerr << "T_min= "         << T_min        << " ";// << std::endl;
    std::cerr << "T_max= "         << T_max        << " ";// << std::endl;
    std::cerr << "scaled_sigma= "  << scaled_sigma << " ";// << std::endl;
    

	// Time the execution of the code 
    int start_s = clock();
    energy = generate_random_points(n_, xyz, n_steps, n_anneals, T_min, T_max, scaled_sigma);
    int stop_s = clock();
    std::cerr << "time= " << (stop_s-start_s) / double(CLOCKS_PER_SEC)*1000 << std::endl;
    
    // Triangulate points 
    traingulate_points(n_, xyz, ltri);

    std::cout << n_ << " " << energy << std::endl;
    for (int i = 0; i < n_; i++)
    {
        std::cout << xyz[3 * i + 0] << " " << xyz[3 * i + 1] << " " << xyz[3 * i + 2] << std::endl;
    }

    std::cout << std::endl;
    for (int i = 0; i < 2*(n_ - 2); i++)
    {
        std::cout << ltri[6*i + 0] << " " << ltri[6*i + 1] << " "<< ltri[6*i + 2] << "||";
        std::cout << ltri[6*i + 3] << " " << ltri[6*i + 4] << " "<< ltri[6*i + 5] << std::endl;
    }

    delete[] xyz;
    delete[] ltri;

    return 0;
}
