#!/bin/bash


#SBATCH --ntasks-per-core=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err


sbatch --time=01:30:00 --job-name="scenario_E" --nodes=1 -A uzh8 strong_scale.sh --cray E a custom 24
