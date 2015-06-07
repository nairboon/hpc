
#ifdef MPI

#include <mpi.h>
#include "parametres.h"



typedef struct mpi_node_t {
    int rank;
    int world_size;


}mpi_node_t;


#define isMaster() mpi_node.rank == 0
#define isSlave() mpi_node.rank > 0


extern mpi_node_t mpi_node;


void init_mpi();


void store_results(long step, const hydroparam_t H, hydrovar_t * Hv);

#else // non mpi code

#define isMaster NULL
#define isSlave 1


#endif