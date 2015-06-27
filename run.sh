#!/bin/bash

#SBATCH --partition=bigmem
#SBATCH --job-name="hello_world_mpi"
#SBATCH --time=00:00:30
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --output=hello_world_mpi.%j.out
#SBATCH --error=hello_world_mpi.%j.err

cd Output
#../hcode/Src/hydro_mono -i ../hcode/Input/input_sedov_100x100.nml

#../hcode/Src/hydro_mono -i ../hcode/Input/input_mini


#export OMP_NUM_THREADS=4
#../hcode/Src/hydro_omp -i ../hcode/Input/input_mini



#LD_PRELOAD=/usr/lib/debug/usr/lib/valgrind/libmpiwrap-amd64-linux.so

#mpirun -n 2 valgrind --leak-check=full --show-leak-kinds=all -v ../hcode/Src/hydro_mpi -i ../hcode/Input/input_mini



if [ "$1" == "--cray" ]; then
    ## on dora
aprun -B time ../hcode/Src/hydro_mpi -i ../hcode/Input/input_mini


 else

 ## at home

if [ "$1" == "--mono" ]; then
 ../hcode/Src/hydro_mono -i ../hcode/Input/input_mini

else
mpirun -n 2 ../hcode/Src/hydro_mpi -i ../hcode/Input/input_mini
fi

fi