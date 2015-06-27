#!/bin/bash

cd Output
#../hcode/Src/hydro_mono -i ../hcode/Input/input_sedov_100x100.nml

#../hcode/Src/hydro_mono -i ../hcode/Input/input_mini


#export OMP_NUM_THREADS=4
#../hcode/Src/hydro_omp -i ../hcode/Input/input_mini


mpirun -n 2 ../hcode/Src/hydro_mpi -i ../hcode/Input/input_mini

#LD_PRELOAD=/usr/lib/debug/usr/lib/valgrind/libmpiwrap-amd64-linux.so

#mpirun -n 2 valgrind --leak-check=full --show-leak-kinds=all -v ../hcode/Src/hydro_mpi -i ../hcode/Input/input_mini

