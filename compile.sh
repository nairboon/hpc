#!/bin/bash



if [ "$1" == "--cray" ]; then
  ln -sf "$(pwd)/hcode/Arch/make_cray" "$(pwd)/hcode/Arch/make.inc"
 else
  ln -sf "$(pwd)/hcode/Arch/make_generique" "$(pwd)/hcode/Arch/make.inc"
fi

cd  hcode/Src

#make mono
make mpi