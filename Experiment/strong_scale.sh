#!/bin/bash


#SBATCH --job-name="hello_world_mpi-strong"
#SBATCH --time=00:01:00
#SBATCH --nodes=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err


# lcm(1,2,3,4,5,6,8) =120

dts() { date +%Y-%m-%d-%H-%M-%S; }

d="$(dts)"

current=$(pwd)

flag=$1

mkdir $d
cd $d




run() {

mkdir $1
cd $1

version=""

echo "running wiht $1"




  if [ "$flag" == "--cray" ]; then
      ## on dora

      if [ $1 -eq "1" ]
         then
            aprun -N $1 -n $1 time  ../../../hcode/Src/hydro_mono -i "$current/../hcode/Input/input_scenario_$2_$3"
         else

        aprun -N $1 -n $1 time ../../../hcode/Src/hydro_mpi -i ../../../hcode/Input/input_scenario_$2_$3

    fi



   else

   if [ $1 -eq "1" ]
   then
        ../../../hcode/Src/hydro_mono -i "$current/../hcode/Input/input_scenario_$2_$3"
   else

        mpirun -n $1 ../../../hcode/Src/hydro_mpi -i ../../../hcode/Input/input_scenario_$2_$3
    fi

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
