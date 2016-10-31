#! /bin/bash -x

rm main.o
g++ -c main.cpp

cd ./src/
rm *.o
gfortran -c stripack.f90
g++ -c random.cpp
g++ -c mc.c
g++ -o test main.o stripack.o mc.o random.o -std=gnu++11 -lgfortran
