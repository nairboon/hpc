#!/bin/bash


#SBATCH --job-name="scenario_T12"
#SBATCH --time=00:15:00
#SBATCH --nodes=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err

sbatch -A uzh8 strong_scale.sh --cray T12 240x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 480x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 720x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 960x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 1200x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 1440x1000 custom 24

sbatch -A uzh8 strong_scale.sh --cray T12 1920x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 2160x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 2400x1000 custom 24
sbatch -A uzh8 strong_scale.sh --cray T12 24000x1000 custom 24
