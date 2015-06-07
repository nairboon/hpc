#!/bin/bash

cd Output
#../hcode/Src/hydro_mono -i ../hcode/Input/input_sedov_100x100.nml

#../hcode/Src/hydro_mono -i ../hcode/Input/input_mini


#export OMP_NUM_THREADS=4
#../hcode/Src/hydro_omp -i ../hcode/Input/input_mini


mpirun -n 3 ../hcode/Src/hydro_mpi