/*
  A simple 2D hydro code
  (C) Romain Teyssier : CEA/IRFU           -- original F90 code
  (C) Pierre-Francois Lavallee : IDRIS      -- original F90 code
  (C) Guillaume Colin de Verdiere : CEA/DAM -- for the C version
*/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "parametres.h"
#include "hydro_funcs.h"
#include "vtkfile_mpi.h"
#include "compute_deltat.h"
#include "hydro_godunov.h"
#include "utils.h"


#include "mpi_helper.h"

hydroparam_t H;
hydrovar_t Hv;                  // nvar
hydrovarwork_t Hvw;             // nvar
hydrowork_t Hw;
unsigned long flops = 0;



int
main(int argc, char **argv)
{

#ifndef MPI
  printf("Only MPI version !!\n");
#endif


#ifdef MPIOMP
  printf("MPIOMP\n");
 #endif


  printf("MPI %d\n",mpi_node.rank);


    init_mpi();



    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, mpi_node.rank, mpi_node.world_size);

    FILE *log;

    if( isMaster() ) {
        printf("Cheffe\n");

        log = fopen("times.log", "w");
        if (log == NULL) {
            fprintf(stderr, "Ouverture du fichier log impossible\n");
            exit(1);
        }

    }


    double dt = 0;
    long nvtk = 0;
    char outnum[80];
    long time_output = 0;

    // double output_time = 0.0;
    double next_output_time = 0;
    double start_time = 0, end_time = 0;
    double start_iter = 0, end_iter = 0;
    double elaps = 0;

    start_time = cclock();
    process_args(argc, argv, &H);

    mpi_node.global_grid_size = H.nx;

    mpi_node.individual_grid_size = H.nx / mpi_node.world_size;

    printf("IGS: %d \n", mpi_node.individual_grid_size);
    // decompose domain into gridsize/num_processors wide chunks
    H.nx = mpi_node.individual_grid_size;

    hydro_init(&H, &Hv);


    post_hydro_init(H);

    // don't know
    PRINTUOLD(H, &Hv);


    printf("Hydro starts - sequential version \n");

    // vtkfile(nvtk, H, &Hv);
    if (H.dtoutput > 0)
    {
        // outputs are in physical time not in time steps
        time_output = 1;
        next_output_time = next_output_time + H.dtoutput;
    }

    /* H.tend = max time of the simulation
     * H.t = sum of timesteps (dt)
     * */
    while ((H.t < H.tend) && (H.nstep < H.nstepmax))
    {
        start_iter = cclock();
        outnum[0] = 0;
        flops = 0;


        /* PART I
         * Compute dTime
         * */
        if ((H.nstep % 2) == 0)
        {
            // Gather all oder so hier
            compute_deltat(&dt, H, &Hw, &Hv, &Hvw);
            if (H.nstep == 0) {
                dt = dt / 2.0;
            }

            MPI_Allreduce(MPI_IN_PLACE, &dt,1,MPI_DOUBLE,MPI_MIN,MPI_COMM_WORLD);
        }

        /* PART II
         * apply godunov
         *
         */



        if ((H.nstep % 2) == 0) {
            hydro_godunov(1, dt, H, &Hv, &Hw, &Hvw);

            share_ghost_cells(H,&Hv);

            hydro_godunov(2, dt, H, &Hv, &Hw, &Hvw);

        } else {
            hydro_godunov(2, dt, H, &Hv, &Hw, &Hvw);

            share_ghost_cells(H,&Hv);

            hydro_godunov(1, dt, H, &Hv, &Hw, &Hvw);
        }

        share_ghost_cells(H,&Hv);



        end_iter = cclock();
        H.nstep++;
        H.t += dt;


/*
        if (flops > 0 && -1) {
            double iter_time = (double) (end_iter - start_iter);
            if (iter_time > 1.e-9) {
                double mflops = (double) flops / (double) 1.e+6 / iter_time;
                sprintf(outnum, "%s {%.3f Mflops} %.3fs", outnum, mflops, iter_time);
            }
        } else {*/

            double iter_time = (double) (end_iter - start_iter);
       // }


        /* PART III
         * gather & write results
         *
         */


            if (time_output == 0) {
                if ((H.nstep % H.noutput) == 0) {

                    store_results(++nvtk, H, &Hv);
                    //vtkfile(++nvtk, H, &Hv);
                    sprintf(outnum, "%s [%04ld]", outnum, nvtk);
                }
            } else {
                if (H.t >= next_output_time) {
                    store_results(++nvtk, H, &Hv);
                    //vtkfile(++nvtk, H, &Hv);
                    next_output_time = next_output_time + H.dtoutput;
                    sprintf(outnum, "%s [%04ld]", outnum, nvtk);
                }
            }


        if( isMaster() ) {

            fprintf(stdout, "--> step=%-4ld %f, %f %f %s\n", H.nstep, H.t, dt, iter_time, outnum);
            unsigned long share,data, rss;
            cscs_read_procstatm(&share,&data,&rss);

            fprintf(log, "%ld, %f, %f, %f, %ld, %ld, %ld, %s\n", H.nstep, H.t, dt, iter_time, share,data, rss, outnum);

            //fprintf(stdout, "%ld %ld\n", size, rss);

        }


    }   // end while loop
    hydro_finish(H, &Hv);
    end_time = cclock();
    elaps = (double) (end_time - start_time);
    timeToString(outnum, elaps);

    fprintf(stdout, "Hydro ends in %ss (%.3lf).\n", outnum, elaps);


    my_cleanup(H);

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
    
