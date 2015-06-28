#!/bin/bash


#SBATCH --job-name="scenario_E"
#SBATCH --time=00:15:00
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err



sbatch --nodes=24 --ntasks-per-node=24 -A uzh8 strong_scale.sh --cray A 24 b
sbatch --nodes=32 --ntasks-per-node=16 -A uzh8 strong_scale.sh --cray A 32 b
sbatch --nodes=64 --ntasks-per-node=8 -A uzh8 strong_scale.sh --cray A 64 b
sbatch --nodes=96 --ntasks-per-node=6 -A uzh8 strong_scale.sh --cray A 96 b