#!/bin/bash


#SBATCH --job-name="scenario_T2"
#SBATCH --time=00:15:00
#SBATCH --nodes=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err



sbatch -A uzh8 strong_scale.sh --cray T2 700x1000 custom 2
sbatch -A uzh8 strong_scale.sh --cray T2 800x1000 custom 2
sbatch -A uzh8 strong_scale.sh --cray T2 900x1000 custom 2
