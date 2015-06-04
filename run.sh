#!/bin/bash

cd Output
#../hcode/Src/hydro_mono -i ../hcode/Input/input_sedov_100x100.nml

mpirun -n 3 ../hcode/Src/hydro_mpi