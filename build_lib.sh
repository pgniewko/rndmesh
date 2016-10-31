#! /bin/bash -x

cd ./src/
rm *.o
rm librndmesh.a
g++ -c rmesh.cpp
gfortran -c stripack.f90
g++ -c random.cpp
g++ -c mc.c

ar -rcs librndmesh.a rmesh.o stripack.o mc.o random.o
ranlib librndmesh.a

sudo cp libsteinhardt.a /usr/local/lib
$ sudo cp steinhardt.h /usr/local/include

#g++ -o test main.o stripack.o mc.o random.o -std=gnu++11 -lgfortran
