DESCRIPTION
==================================================
The ```rndmesh``` library, using Simulated Annealing, generates points on a sphere and
then triangulates the surface with the vertices defined by those points.


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

COMPILING AND INSTALLATION - LINUX
==================================================

* gfortran
On Ubuntu you can install it by running:
```
apt-get install gfortran
```

* compile by:
```
./build_lib.sh
```


USAGE
=====
The lirbary is open-source. If you want to cite the library in any pubished work please contact me at
pawel.gniewek@berkeley.edu for an information about credits. 


COPYRIGHT NOTICE
================
Copyright (C) 2014-2016,  Pawel Gniewek  
Email  : pawel.gniewek@berkeley.edu  
All rights reserved.  
License: BSD  


ACKNOWLEDGMENTS
===============
Delaunay Triangulation on a Sphere follows STRIPACK implementation. 
Please refer to the following paper for more details:
    
Robert Renka,
Algorithm 772: STRIPACK: Delaunay Triangulation and Voronoi Diagram on the Surface of a Sphere,
ACM Transactions on Mathematical Software,
Volume 23, Number 3, September 1997, pages 416-434.
