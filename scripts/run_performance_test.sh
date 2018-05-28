#! /bin/bash -x

rm timing.dat 2> /dev/null
touch timing.dat

../run_rndmesh 50    50 50 0.1 100 0.141421 > tmp.dat 2>>timing.dat 
../run_rndmesh 100   50 50 0.1 100 0.100000 > tmp.dat 2>>timing.dat
../run_rndmesh 500   50 50 0.1 100 0.044721 > tmp.dat 2>>timing.dat
../run_rndmesh 1000  50 50 0.1 100 0.031622 > tmp.dat 2>>timing.dat
../run_rndmesh 5000  50 50 0.1 100 0.014142 > tmp.dat 2>>timing.dat
../run_rndmesh 10000 50 50 0.1 100 0.010000 > tmp.dat 2>>timing.dat
../run_rndmesh 50000 50 50 0.1 100 0.004472 > tmp.dat 2>>timing.dat


../run_rndmesh 50    100 50 0.1 100 0.141421 > tmp.dat 2>>timing.dat
../run_rndmesh 100   100 50 0.1 100 0.100000 > tmp.dat 2>>timing.dat
../run_rndmesh 500   100 50 0.1 100 0.044721 > tmp.dat 2>>timing.dat
../run_rndmesh 1000  100 50 0.1 100 0.031622 > tmp.dat 2>>timing.dat
../run_rndmesh 5000  100 50 0.1 100 0.014142 > tmp.dat 2>>timing.dat
../run_rndmesh 10000 100 50 0.1 100 0.010000 > tmp.dat 2>>timing.dat
../run_rndmesh 50000 100 50 0.1 100 0.004472 > tmp.dat 2>>timing.dat


../run_rndmesh 50    100 100 0.1 100 0.141421 > tmp.dat 2>>timing.dat
../run_rndmesh 100   100 100 0.1 100 0.100000 > tmp.dat 2>>timing.dat
../run_rndmesh 500   100 100 0.1 100 0.044721 > tmp.dat 2>>timing.dat
../run_rndmesh 1000  100 100 0.1 100 0.031622 > tmp.dat 2>>timing.dat
../run_rndmesh 5000  100 100 0.1 100 0.014142 > tmp.dat 2>>timing.dat
../run_rndmesh 10000 100 100 0.1 100 0.010000 > tmp.dat 2>>timing.dat
../run_rndmesh 50000 100 100 0.1 100 0.004472 > tmp.dat 2>>timing.dat

rm tmp.dat
