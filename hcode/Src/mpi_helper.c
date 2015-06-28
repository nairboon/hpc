#include <stdio.h>

#include <stddef.h>
#include <malloc.h>

#include "mpi_helper.h"

#include "vtkfile_mpi.h"
mpi_node_t mpi_node;

#define VARIDOFFSET 42

MPI_Datatype mpi_sparse_hydroparam;
typedef struct sparse_hydroparam_t {
    long imin, imax, jmin, jmax, nx, ny;
}sparse_hydroparam;


double *resHv;

double* results[4];
double *sb[4];


void init_mpi(hydroparam_t H) {

    MPI_Init(NULL, NULL);


    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_node.world_size);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_node.rank);


}

void post_hydro_init(const hydroparam_t H) {

    if ( isMaster() ) {
        int n = H.nvar * H.nxt * H.nyt;

        resHv = malloc(n * mpi_node.world_size *sizeof(double));
    }

    for(int v=0;v<H.nvar;v++) {

        sb[v] = malloc(H.jmax * 2 * sizeof(double));
        results[v] = malloc(H.jmax * 2 * sizeof(double));
    }

}

void my_cleanup(const hydroparam_t H) {
    for(int v=0;v<H.nvar;v++) {
        free(results[v]);
         free(sb[v]);
    }
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


    MPI_Barrier(MPI_COMM_WORLD);

    int n = H.nvar * H.nxt * H.nyt;

    double resHv[n * mpi_node.world_size];

    MPI_Gather(Hv->uold, n, MPI_DOUBLE, &resHv, n, MPI_DOUBLE, 0 ,MPI_COMM_WORLD );


    if ( isMaster() ) {

        hydrovar_t HvG;
        HvG.uold = resHv;

        vtkfile(step, H, &HvG);

    }

}


void _share_ghost_send(hydroparam_t H, hydrovar_t * Hv,int var, int xstart, int target,MPI_Request *request, double *r) {



    // bottom -up
    int i=0;
    for(int j=0;j<H.jmax;j++) {

        r[i++] = Hv->uold[IHv(xstart  , j, var)];
        r[i++] = Hv->uold[IHv(xstart + 1, j, var)];
    }


    MPI_Isend (r, H.jmax*2, MPI_DOUBLE,
               target, var + VARIDOFFSET, MPI_COMM_WORLD, request); // 42== VARNAME TAG OFFSET

}

void _share_ghost_receive(hydroparam_t H, hydrovar_t * Hv,int var, int target,double *r, MPI_Request *request) {


    MPI_Status status;
    int count;

    MPI_Irecv (r, H.jmax*2, MPI_DOUBLE, target, var + VARIDOFFSET, MPI_COMM_WORLD,
               request);

}

void share_right(hydroparam_t H, hydrovar_t * Hv) {

    MPI_Request requests_recv[4];

    MPI_Request requests_send[4];

    MPI_Status statuses[4];


    for (int v = 0; v < H.nvar; v++) {
        _share_ghost_send(H, Hv, v, H.nx, mpi_node.rank + 1, &requests_send[v], sb[v]);
        _share_ghost_receive(H, Hv, v, mpi_node.rank + 1, results[v], &requests_recv[v]);
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


   // printf("Updated my right side\n");
}


void share_left(hydroparam_t H, hydrovar_t * Hv) {



    MPI_Request requests_recv[4];

    MPI_Request requests_send[4];

    MPI_Status statuses[4];


    for(int v=0;v<H.nvar;v++) {


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


    }
       // printf("Updated my left side\n");

}


void share_ghost_cells(hydroparam_t H, hydrovar_t * Hv) {

    if ( isMaster() ) { // only share right side

       // printf("I'm master, share right\n");

        share_right(H,Hv);

    } else if ( mpi_node.rank == (mpi_node.world_size -1) ) {

        //printf("I'm right end, share left\n");

        share_left(H,Hv);

    } else { //share left & right
       // printf("I'm in the middle...\n");

        share_left(H,Hv);
        share_right(H,Hv);

    }
}
