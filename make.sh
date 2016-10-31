#! /bin/bash -x

rm test
gfortran -c stripack.f90
g++ -c main.cpp
g++ -c random.cpp
g++ -c mc.c
g++ -o test main.o stripack.o mc.o random.o -std=gnu++11 -lgfortran
