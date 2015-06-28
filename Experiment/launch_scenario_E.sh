#!/bin/bash


#SBATCH --job-name="scenario_E"
#SBATCH --time=00:15:00
#SBATCH --nodes=8
#SBATCH --ntasks-per-core=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err



sbatch -A uzh8 strong_scale.sh --cray E a custom 16