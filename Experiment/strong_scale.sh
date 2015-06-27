#!/bin/bash


#SBATCH --job-name="hello_world_mpi_strong"
#SBATCH --time=00:01:00
#SBATCH --nodes=1
#SBATCH --output=hello_world_mpi-s.%j.out
#SBATCH --error=hello_world_mpi-s.%j.err


# lcm(1,2,3,4,6,8,12) = 24
# lcm(1, 2, 3, 4, 6, 8, 10, 12, 16, 20, 24, 30, 40, 48) = 240 - big set

dts() { date +%Y-%m-%d-%H-%M-%S; }

current=$(pwd)


os=$1
scenario=$2
variant=$3
size=$4

if [ "$size" != "b" ]; then
    size="s"
fi


big="1 2 3 4 6 8 10 12 16 20 24 30 40 48"
small="1 2 3 4 6 8 12"

echo "$os $scenario $variant $size"

d="${scenario}_${variant}_${size}/$(dts)"

mkdir -p "$d"
cd $d




run() {

mkdir $1
cd $1

version=""

echo "running with $1"


  if [ "$os" == "--cray" ]; then
      ## on dora
      if [ $1 -eq "1" ]
         then
            aprun -N 1 -n 1 time  "$current/../hcode/Src/hydro_mono" -i "$current/../hcode/Input/input_scenario_$2_$3"
         else

        aprun -N $1 -n $1 time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"

    fi



   else

   if [ $1 -eq "1" ]
   then
        "$current/../hcode/Src/hydro_mono" -i "$current/../hcode/Input/input_scenario_$2_$3"
   else

        mpirun -n $1 "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"
    fi

fi

cd ..
}



run_experiment() {

for n in $1; do
  echo "Run with $n process"

    run $n $scenario $variant


done

}




if [ "$size" == "b" ]; then
    run_experiment "$big"
else
    run_experiment "$small"
fi
