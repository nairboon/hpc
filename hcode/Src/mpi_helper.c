#include <stdio.h>


#include "mpi_helper.h"

#include "vtkfile.h"
mpi_node_t mpi_node;

#define VARIDOFFSET 42

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



        H.imax = H.imax * mpi_node.world_size;

        printf("Master from  i: %d ->%d\t j: %d ->%d\n", H.imin, H.imax, H.jmin, H.jmax);

        //H.imax = (H.nx * mpi_node.world_size) + ExtraLayerTot; //(H.imax - ExtraLayerTot) * mpi_node.world_size;



        //printf(" %d", (H.nx * mpi_node.world_size) + ExtraLayerTot );
        H.nx = H.nx * mpi_node.world_size;


        hydrovar_t HvG;
        HvG.uold = resHv;

        vtkfile(step, H, &HvG);
    } else { // slave

        printf("Slave %d sent from  i: %d ->%d\t j: %d ->%d\n", mpi_node.rank, H.imin, H.imax, H.jmin, H.jmax);

        //printf("I %d sent %d, %d\n", mpi_node.rank, shp.imin, shp.imax);
    }

}


void _share_ghost_send(hydroparam_t H, hydrovar_t * Hv,int var, int xstart, int target,MPI_Request *request, double *r) {



    // bottom -up
    int i=0;
    for(int j=0;j<H.jmax;j++) {

        r[i++] = Hv->uold[IHv(xstart  , j, var)];
        r[i++] = Hv->uold[IHv(xstart + 1, j, var)];
    }

    if (var == 0 || var == IP) {
        printf("Var: %d\n", var);
        for (int j = 0; j < H.jmax * 2; j++) {
            printf("%f %f \n", r[j], r[j + 1]);
            j++;
        }
    }



    MPI_Isend (r, H.jmax*2, MPI_DOUBLE,
               target, var + VARIDOFFSET, MPI_COMM_WORLD, request); // 42== VARNAME TAG OFFSET

}

void _share_ghost_receive(hydroparam_t H, hydrovar_t * Hv,int var, int target,double *r, MPI_Request *request) {


    MPI_Status status;
    int count;

    MPI_Irecv (r, H.jmax*2, MPI_DOUBLE, target, var + VARIDOFFSET, MPI_COMM_WORLD,
               request);

    //MPI_Wait (request, &status);
    //MPI_Get_count (&status, MPI_DOUBLE, &count);

    /*printf("REcevied\n");
    for(int j=0;j<count;j++) {
        printf("%f %f \n", r[j],r[j+1]);
        j++;
    }*/

}

void share_right(hydroparam_t H, hydrovar_t * Hv) {


    MPI_Request requests_recv[4];

    MPI_Request requests_send[4];

    MPI_Status statuses[4];



    double* results[4];

    for(int v=0;v<H.nvar;v++) {

        double *sb = malloc(H.jmax*2 *sizeof(double));

        _share_ghost_send(H, Hv, v, H.nx, mpi_node.rank+1,&requests_send[v],sb);

        results[v] = malloc(H.jmax*2 * sizeof(double));

        _share_ghost_receive(H, Hv, v, mpi_node.rank+1, results[v], &requests_recv[v]);


    }



   // printf("Big wait...\n");

    MPI_Waitall(4,requests_send,statuses);



    for(int v=0;v<H.nvar;v++) {
        MPI_Status status;
        int count;

       // printf("waiting for %d...\t",v);

        MPI_Wait (&requests_recv[v], &status);
        MPI_Get_count (&status, MPI_DOUBLE, &count);

      //  printf("done: %d\n",count);



        for(int j=0;j<H.jmax;j++) {

            Hv->uold[IHv(H.nx + ExtraLayerTot - 2, j, v)] = results[v][j*2];
            Hv->uold[IHv(H.nx + ExtraLayerTot - 1, j, v)] = results[v][(j*2)+1];

//            j++;
        }

    }


    printf("Updated my right side\n");
}


void share_left(hydroparam_t H, hydrovar_t * Hv) {



    MPI_Request requests_recv[4];

    MPI_Request requests_send[4];

    MPI_Status statuses[4];

    double* results[4];
    double *sb[4];

    for(int v=0;v<H.nvar;v++) {
        sb[v] = malloc(H.jmax*2 *sizeof(double));

        results[v] = malloc(H.jmax*2 * sizeof(double));

        _share_ghost_receive(H, Hv, v, mpi_node.rank-1, results[v], &requests_recv[v]);

        _share_ghost_send(H, Hv, v, ExtraLayer , mpi_node.rank-1, &requests_send[v],sb[v]);


    }


    //printf("Big wait...\n");

    MPI_Waitall(4,requests_send,statuses);



    for(int v=0;v<H.nvar;v++) {
        MPI_Status status;
        int count;

       // printf("waiting for %d...\t",v);

        MPI_Wait (&requests_recv[v], &status);
        MPI_Get_count (&status, MPI_DOUBLE, &count);

      //  printf("done: %d\n",count);



        for(int j=0;j<H.jmax;j++) {

            Hv->uold[IHv(0, j, v)] = results[v][j*2];
            Hv->uold[IHv(1, j, v)] = results[v][(j*2)+1];

            //j++;
        }

        free(results[v]);
        free(sb[v]);
    }
        printf("Updated my left side\n");

}


void share_ghost_cells(hydroparam_t H, hydrovar_t * Hv) {

    if ( isMaster() ) { // only share right side

        printf("I'm master, share right\n");

        share_right(H,Hv);

    } else if ( mpi_node.rank == (mpi_node.world_size -1) ) {

        printf("I'm right end, share left\n");

        share_left(H,Hv);

    } else { //share left & right
        printf("I'm in the middle...\n");

        share_left(H,Hv);
        share_right(H,Hv);

    }
}