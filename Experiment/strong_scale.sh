#!/bin/bash

# lcm(1,2,3,4,5,6,8) =120

dts() { date +%Y-%m-%d-%H-%M-%S; }

d="$(dts)"

current=$(pwd)


mkdir $d
cd $d




run() {

mkdir $1
cd $1

version=""

echo "running wiht $1"

if [ $1 -eq "1" ]
then
     ../../../hcode/Src/hydro_mono -i "$current/../hcode/Input/input_scenario_$2_$3"
else
  mpirun -n $1 ../../../hcode/Src/hydro_mpi -i ../../../hcode/Input/input_scenario_$2_$3
fi

cd ..
}


run 1 A sq

run 2 A sq
run 3 A sq
run 4 A sq
run 5 A sq
run 6 A sq
run 8 A sq
