#!/bin/bash


#SBATCH --job-name="scenario_S"
#SBATCH --time=00:20:00
#SBATCH --ntasks-per-core=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err



sbatch --nodes=64 -A uzh8 strong_scale.sh --cray S ss b