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
sbatch -A uzh8 strong_scale.sh --cray D 192x192 scd
sbatch -A uzh8 strong_scale.sh --cray D 288x128 scd
sbatch -A uzh8 strong_scale.sh --cray D 384x96 scd
sbatch -A uzh8 strong_scale.sh --cray D 576x64 scd
sbatch -A uzh8 strong_scale.sh --cray D 768x48 scd
sbatch -A uzh8 strong_scale.sh --cray D 1152x32 scd
sbatch -A uzh8 strong_scale.sh --cray D 1536x24 scd
sbatch -A uzh8 strong_scale.sh --cray D 2304x16 scd
sbatch -A uzh8 strong_scale.sh --cray D 3072x12 scd