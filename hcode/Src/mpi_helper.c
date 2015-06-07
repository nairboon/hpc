
#include "mpi_helper.h"


mpi_node_t mpi_node;



void init_mpi() {

    MPI_Init(NULL, NULL);


    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_node.world_size);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_node.rank);


}
