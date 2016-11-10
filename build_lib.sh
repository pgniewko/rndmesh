#! /bin/bash -x

cd ./src/
rm *.o
rm librndmesh.a

g++ -c rndmesh.cpp
gfortran -c stripack.f90
g++ -c random.cpp
g++ -c mc.c

ar -rcs librndmesh.a rndmesh.o stripack.o mc.o random.o
ranlib librndmesh.a
sudo cp librndmesh.a /usr/local/lib
sudo cp rndmesh.h /usr/local/include
sudo cp mc.h /usr/local/include
sudo cp random.h /usr/local/include

cd ..
g++ -o rndmesh_test main.cpp -std=gnu++11 -Isrc/ -lrndmesh -lgfortran
