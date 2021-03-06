# Link make.inc with the targeted architecture in Arch directory
# For example : ln -s make_sp6 make.inc
#--------------------------------------
include ../Arch/make.inc


SRC=cmpflx.c conservar.c equation_of_state.c hydro_godunov.c parametres.c riemann.c trace.c compute_deltat.c constoprim.c hydro_funcs.c make_boundary.c qleftright.c slope.c utils.c

OBJ = $(patsubst %.c, %.o, ${SRC})

default: mono

all: mono mpi omp mpiomp

mono: CCOMP = $(CC)
mono: CCOMPFLAGS = -DMONO -O4
mono: LINKFLAGS = $(LDFLAGS)
mono:
	$(CCOMP) $(CCOMPFLAGS)  $(LINKFLAGS) $(SRC) vtkfile.c main.c -o hydro_mono -lm

mpi: CCOMP = $(MPICC)
mpi: CCOMPFLAGS = $(FFLAGS) $(CFLAGS_MPI)
mpi: LINKFLAGS = $(LDFLAGS) $(LDFLAGS_MPI)
mpi: $(SRC)
	$(CCOMP) $(CCOMPFLAGS) -DMPI $(LINKFLAGS) $(SRC)  vtkfile_mpi.c mpi_helper.c main_mpi.c -o hydro_mpi -lm

omp: CCOMP = $(CC)
omp: CCOMPFLAGS = $(FFLAGS) $(FFLAGS_OMP)
omp: LINKFLAGS = $(LDFLAGS) $(LDFLAGS_OMP)
omp: $(SRC)
	$(CCOMP) -DOMP -fopenmp $(LINKFLAGS) $(SRC) main.c -o hydro_omp -lm

mpiomp: CCOMP = $(MPICC)
mpiomp: CCOMPFLAGS = $(FFLAGS) $(FFLAGS_MPI) $(FFLAGS_OMP)
mpiomp: LINKFLAGS = $(LDFLAGS) $(LDFLAGS_MPI) $(LDFLAGS_OMP)
mpiomp: $(OBJ)
	$(CCOMP) -DMPI -DOMP $(LINKFLAGS) $(OBJ) main_mpi.c -o hydro_mpiomp -lm


clean   :
	rm -f *.o core* hydro_mono hydro_mpi hydro_omp hydro_mpiomp

cleanall   : clean
	rm -f *~ *.vts *.out

.SUFFIXES:  .o .c

.c.o    :
	${CCOMP} ${CCOMPFLAGS} -c $<

# DO NOT DELETE

cmpflx.o: parametres.h utils.h cmpflx.h
compute_deltat.o: parametres.h compute_deltat.h utils.h equation_of_state.h
conservar.o: parametres.h utils.h conservar.h
constoprim.o: parametres.h constoprim.h utils.h
equation_of_state.o: equation_of_state.h
equation_of_state.o: utils.h parametres.h
hydro_funcs.o: utils.h parametres.h hydro_funcs.h
hydro_godunov.o: parametres.h hydro_godunov.h
hydro_godunov.o: hydro_funcs.h utils.h make_boundary.h cmpflx.h conservar.h
hydro_godunov.o: equation_of_state.h qleftright.h constoprim.h riemann.h
hydro_godunov.o: trace.h slope.h
main.o: parametres.h hydro_funcs.h vtkfile.h
main.o: compute_deltat.h hydro_godunov.h utils.h
make_boundary.o: parametres.h make_boundary.h
make_boundary.o: utils.h
parametres.o: parametres.h
qleftright.o: parametres.h utils.h qleftright.h
riemann.o: parametres.h utils.h riemann.h
slope.o: parametres.h utils.h slope.h
trace.o: parametres.h utils.h trace.h
utils.o: utils.h parametres.h
vtkfile.o: parametres.h utils.h
vtkfile.o: vtkfile.h
cmpflx.o: utils.h parametres.h
constoprim.o: utils.h parametres.h
equation_of_state.o: utils.h parametres.h
hydro_funcs.o: parametres.h
utils.o: parametres.h
