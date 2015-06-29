#!/bin/bash

rsync -av -e "ssh -A course07@ela.cscs.ch ssh" --include='*.log' --include='*/' --exclude='*' course07@dora:./hydrohpc1b/hpc/Experiment ./mirror