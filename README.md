DESCRIPTION
==================================================
The ```rndmesh``` library, using Simulated Annealing Monte Carlo, generates points on a sphere and
triangulates the surface with the vertices defined by those points.


GETTING THE CODE
==================================================
* To get the code:
```
git clone git@bitbucket.org:pawelgniewek/rndmesh.git
```

* To obtain the most recent version of the code:
```
git pull origin master
```

COMPILING AND INSTALLATION - UNIX
==================================================

* build a library by typing:
```
make
```

* install in /usr/local directory (requires a root access):
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

* An example of how to use the rndmesh library: 
```C++
#include <rndmesh.h>

...

// Initialize pseudo-random number generator
srand (time(NULL));
unsigned long init[4]={(unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand()}, length=4;
init_by_array(init, length);

// Provide parameters for the simulation
int n_ = 1000;              // Simulate 1000 particles on the sphere
int n_steps = 100;          // 100 MC steps per each temperatur annealing
int n_anneals = 100;        // 100 annealings 
double T_min = 1.0;         // END temperture
double T_max = 500.0;       // START temperature
double scaled_sigma = 0.1; // Particle's radius 

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
Email  : pawel.gniewek@berkeley.edu  
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
