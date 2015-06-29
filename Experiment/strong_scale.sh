#!/bin/bash


#SBATCH --time=00:15:00

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
custom=$5


big="8 10 16 20 24 30 40 48 64 128 192 256 320 384 512 640 768 1280 1536 6 3 1 2"

#big="8 24 64 128"

small="1 2 3 4 6 8 12"

huge="1 2 4 8 16 24 48 96 192"

scd="12"

#small="1 2 3 4 6"

echo "$os $scenario $variant $size $custom"

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
            aprun -n 1 time  "$current/../hcode/Src/hydro_mono" -i "$current/../hcode/Input/input_scenario_$2_$3"
         else

    if [ "$scenario" -eq "A" ]
   then
           aprun -n $1 -N $custom time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"

    elif [  "$scenario" -eq "T2" ]; then
        aprun -N 1 --ntasks=$1 -S 1 -ss --cpu_bind=sockets time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"

    elif [  "$scenario" -eq "T12" ]; then
        aprun -N 1 --ntasks=$1 -S 12 -ss -m plane=24:block --cpu_bind=cores time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"


        elif [  "$scenario" -eq "S" ]; then
        aprun -N 64 --ntasks=$1 -S 12 -ss -m plane=24:block --cpu_bind=cores time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"

        elif [  "$scenario" -eq "E" ]; then
        aprun -N 32 --ntasks=$1 -S 12 -ss -m plane=24:block --cpu_bind=cores time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"
        else

        aprun -n $1 time "$current/../hcode/Src/hydro_mpi" -i "$current/../hcode/Input/input_scenario_$2_$3"
        fi
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
elif [ "$size" == "h" ]; then
    run_experiment "$huge"
elif [ "$size" == "scd" ]; then
    run_experiment "$scd"
elif [ "$size" == "custom" ]; then
    run_experiment "$custom"
else
    run_experiment "$small"
fi
