#! /bin/bash -x

cd ./src/
rm *.o
rm librndmesh.a

g++-4.9 -c rndmesh.cpp
gfortran-4.9 -c stripack.f90
g++-4.9 -c random.cpp
g++-4.9 -c mc.c

ar -rcs librndmesh.a rndmesh.o stripack.o mc.o random.o
ranlib librndmesh.a
sudo cp librndmesh.a /usr/local/lib
sudo cp rndmesh.h /usr/local/include

cd ..
g++-4.9 -o rndmesh_test main.cpp -std=gnu++11 -Isrc/ -lrndmesh -lgfortran
