#!/bin/bash


#SBATCH --job-name="scenario_E"
#SBATCH --time=00:15:00
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err



sbatch --nodes=24 -A uzh8 strong_scale.sh --cray A 24 b 24
sbatch --nodes=32 -A uzh8 strong_scale.sh --cray A 32 b 16
sbatch --nodes=64 -A uzh8 strong_scale.sh --cray A 64 b 8
sbatch --nodes=96 -A uzh8 strong_scale.sh --cray A 96 b 6