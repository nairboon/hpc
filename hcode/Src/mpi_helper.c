#include <stdio.h>


#include "mpi_helper.h"

#include "vtkfile.h"
mpi_node_t mpi_node;


MPI_Datatype mpi_sparse_hydroparam;
typedef struct sparse_hydroparam_t {
    long imin, imax, jmin, jmax, nx, ny;
}sparse_hydroparam;


void init_mpi() {

    MPI_Init(NULL, NULL);


    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_node.world_size);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_node.rank);


    /*
     *  Our types
     */






    MPI_Datatype types[] = {MPI_LONG,MPI_LONG,MPI_LONG,MPI_LONG,MPI_LONG,MPI_LONG};
    int          blocklengths[] = {1,1,1,1,1,1};
    MPI_Aint     offsets[6];

    offsets[0] = offsetof(sparse_hydroparam, imin);
    offsets[1] = offsetof(sparse_hydroparam, imax);
    offsets[2] = offsetof(sparse_hydroparam, jmin);
    offsets[3] = offsetof(sparse_hydroparam, jmax);
    offsets[4] = offsetof(sparse_hydroparam, nx);
    offsets[5] = offsetof(sparse_hydroparam, ny);


    MPI_Type_create_struct(6, blocklengths, offsets, types, &mpi_sparse_hydroparam);
    MPI_Type_commit(&mpi_sparse_hydroparam);



}


void store_results(long step, hydroparam_t H, hydrovar_t * Hv) {

    /*
     * from H (param_t) we need:
     *  nx, ny, jmin, jmax, imin, imax
     *
     * actually we don't  -.-, it's the same for all of them
     *
     * from Hv we need: for real
     *  uold, size: H->nvar * H->nxt * H->nyt
     */

/*
    sparse_hydroparam shp;
    shp.imin = H.imin;
    shp.imax = H.imax;

    shp.nx = H.nx;
    shp.ny = H.ny;


    sparse_hydroparam res[mpi_node.world_size];

    MPI_Gather(&shp, 1, mpi_sparse_hydroparam, &res, 1, mpi_sparse_hydroparam, 0 ,MPI_COMM_WORLD );
*/


    int n = H.nvar * H.nxt * H.nyt;

    double resHv[n * mpi_node.world_size];
    MPI_Gather(Hv->uold, n, MPI_DOUBLE, &resHv, n, MPI_DOUBLE, 0 ,MPI_COMM_WORLD );



    if ( isMaster() ) {

        //for(int i=0;i<mpi_node.world_size;i++)
        //    resHv[i]->uold = (double *) calloc(n, sizeof(double));



        //printf(" Master received: \n");
        //for(int i=0;i<mpi_node.world_size;i++)
        //    printf(" %d - %d \n", res[i].ny, res[i].nx);

        H.imax = H.imax * mpi_node.world_size;

        printf("Master from  i: %d ->%d\t j: %d ->%d\n", H.imin, H.imax, H.jmin, H.jmax);

        //H.imax = (H.nx * mpi_node.world_size) + ExtraLayerTot; //(H.imax - ExtraLayerTot) * mpi_node.world_size;



        //printf(" %d", (H.nx * mpi_node.world_size) + ExtraLayerTot );
        H.nx = H.nx * mpi_node.world_size;


        hydrovar_t HvG;
        HvG.uold = resHv;

        vtkfile(step, H, &HvG);
    } else { // slave

        printf("Slave from  i: %d ->%d\t j: %d ->%d\n", H.imin, H.imax, H.jmin, H.jmax);

        //printf("I %d sent %d, %d\n", mpi_node.rank, shp.imin, shp.imax);
    }



}