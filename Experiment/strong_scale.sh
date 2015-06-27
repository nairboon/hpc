#!/bin/bash

# lcm(1,2,3,4,5,6,8) =120

dts() { date +%Y-%m-%d-%H-%M-%S; }

d="$(dts)"

mkdir $d
cd $d


run() {

mkdir $1
cd $1

version=""

echo "running wiht $1"

if [ $1 -eq "1" ]
then
  version="mono"
else
  version="mpi"
fi


mpirun -n $1 ../../../hcode/Src/hydro_$version -i ../../../hcode/Input/input_scenario_$2_$3
cd ..
}


#run 1 A seq

run 2 A sq
run 3 A sq
run 4 A sq
run 5 A sq
run 6 A sq
run 8 A sq
