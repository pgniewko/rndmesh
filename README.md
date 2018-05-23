DESCRIPTION
==================================================
The ```rndmesh``` library, using Simulated Annealing Monte Carlo, generates points on a sphere and
triangulates the surface with the vertices defined by these points.


GETTING THE CODE
==================================================
* To get the code:
```
git clone git@github.com:pgniewko/rndmesh.git
```

* To obtain the most recent version of the code:
```
git pull origin master
```

COMPILING AND INSTALLATION - UNIX
==================================================

* To build a library by typing:
```
make
```

* To install in /usr/local directory (requires a root access):
```
make install
```

USAGE - EXE file
==================================================

* To compile main.cpp file run in a command line (once you've installed the library):
```
make exe
```

The command will result with a binary file run_rndmesh. Execute this file to run an exemplary simulation.

* To run the exe file type in a command line (refer the code snippet below for the meaning of the arguments):
```
./run_rndmesh  100 50 50 0.1 10 0.25
```

* An example of how to use the rndmesh library: 
```C++
#include <rndmesh.h>

...

// Initialize pseudo-random number generator
srand (time(NULL));
unsigned long init[4]={(unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand()}, length=4;
init_by_array(init, length);

// Provide parameters for the simulation
int n_ = 1000;              // 1st: Simulate 1000 particles on the sphere
int n_steps = 100;          // 2nd: 100 MC steps per each temperatur annealing
int n_anneals = 100;        // 3rd: 100 annealings 
double T_min = 1.0;         // 4th: END temperture
double T_max = 500.0;       // 5th: START temperature
double scaled_sigma = 0.1;  // 6th: Particle's radius 

// Define auxiliary data
int nrow = 6;
double* xyz = new double[3 * n_];
int* ltri = new int[nrow * 2 * (n_ - 2)];


double energy;
// Run MC simulation and get points coordinates
energy = generate_random_points(n_, xyz, n_steps, n_anneals, T_min, T_max, scaled_sigma);
// Triangulate the sphere
traingulate_points(n_, xyz, ltri);

...

```

LICENSE
=====
The library is open-source. If you want to cite the library in any published work please contact me at
pawel.gniewek@berkeley.edu for an information about credits or check my [website](http://meetpg.pl/notes.html).

COPYRIGHT NOTICE
================
Copyright (C) 2016-2018,  Pawel Gniewek  
Email  : gniewko.pablo@gmail.com     
All rights reserved.   
License: BSD  


REFERENCES
===============
Delaunay Triangulation on a Sphere follows STRIPACK implementation. 
Please refer to the following paper for more details:
    
Robert Renka,
Algorithm 772: STRIPACK: Delaunay Triangulation and Voronoi Diagram on the Surface of a Sphere,
ACM Transactions on Mathematical Software,
Volume 23, Number 3, September 1997, pages 416-434.
