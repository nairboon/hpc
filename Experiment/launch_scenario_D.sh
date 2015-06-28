#!/bin/bash


#SBATCH --job-name="scenario_D"
#SBATCH --time=00:15:00
#SBATCH --nodes=6
#SBATCH --ntasks-per-core=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err



sbatch -A uzh8 strong_scale.sh --cray D 24x1536 scd
sbatch -A uzh8 strong_scale.sh --cray D 72x512 scd
sbatch -A uzh8 strong_scale.sh --cray D 96x384 scd
sbatch -A uzh8 strong_scale.sh --cray D 144x256 scd