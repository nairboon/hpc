/*
  A simple 2D hydro code
  (C) Romain Teyssier : CEA/IRFU           -- original F90 code
  (C) Pierre-Francois Lavallee : IDRIS      -- original F90 code
  (C) Guillaume Colin de Verdiere : CEA/DAM -- for the C version
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "parametres.h"
#include "utils.h"
#include "vtkfile.h"

#include "mpi_helper.h"

void
vtkfile(long step, const hydroparam_t H, hydrovar_t * Hv)
{
    char name[160];
    FILE *fic;
    long i, j, nv;

    WHERE("vtkfile");
    sprintf(name, "outputvtk_%05ld.vts", step);
    fic = fopen(name, "w");
    if (fic == NULL) {
        fprintf(stderr, "Ouverture du fichier %s impossible\n", name);
        exit(1);
    }
    fprintf(fic, "<?xml version=\"1.0\"?>\n");
    fprintf(fic, "<VTKFile type=\"StructuredGrid\">\n");
    fprintf(fic, "<StructuredGrid WholeExtent=\" %ld %ld %ld %ld %ld %ld\">\n", (long)0,
            H.nx, (long)0, H.ny, (long)0, (long)0);
    fprintf(fic, "<Piece Extent=\" %ld %ld %ld %ld %ld %ld\">\n", (long)0, H.nx, (long)0, H.ny, (long)0, (long)0);
    fprintf(fic, "<Points>\n");
    fprintf(fic,
            "<DataArray type=\"Float32\" format=\"ascii\" NumberOfComponents=\"3\">\n");
    for (j = 0; j < H.ny + 1; j++) {
        for (i = 0; i < H.nx + 1; i++) {
            fprintf(fic, "%f %f %f\n", i * H.dx, j * H.dx, 0.0);
        }
    }
    fprintf(fic, "</DataArray>\n");
    fprintf(fic, "</Points>\n");
    name[0] = 0;
    for (nv = 0; nv < IP; nv++) {
        if (nv == ID)
            sprintf(name, "%s varID", name);
        if (nv == IU)
            sprintf(name, "%s varIU", name);
        if (nv == IV)
            sprintf(name, "%s varIV", name);
        if (nv == IP)
            sprintf(name, "%s varIP", name);
    }

    long individual_grid_size = H.nx / mpi_node.world_size;


    // declaration of the variable list
    fprintf(fic, "<CellData Scalars=\"%s\">\n", name);
    name[0] = 0;
    for (nv = 0; nv <= IP; nv++) {
        if (nv == ID)
            sprintf(name, "varID");
        if (nv == IU)
            sprintf(name, "varIU");
        if (nv == IV)
            sprintf(name, "varIV");
        if (nv == IP)
            sprintf(name, "varIP");

        //Definition of the cell values
        fprintf(fic, "<DataArray type=\"Float32\" Name=\"%s\" format=\"ascii\">\n", name);


        int offset = H.nvar * H.nxt * H.nyt;

        // the image is the interior of the computed domain
        for (j = H.jmin + ExtraLayer; j < H.jmax - ExtraLayer; j++) {
            int domain = 0; // start with "master domain"
           int x=0;
            for (i = H.imin + ExtraLayer; i < H.imax - ExtraLayer; i++) {

                if(x >=individual_grid_size) {
                    //printf("cross domain at %d\n",i); // skipp ghost boxes
                 x=0;
                    domain++;
                    i+=4;
                }
                fprintf(fic, "%lf ", Hv->uold[IHv(i + (domain * offset), j, nv)]);
                //printf("%d %d %d \t %d\n",i,j,nv,x);
                x++;
            }
            fprintf(fic, "\n");
        }
        fprintf(fic, "</DataArray>\n");
    }
    fprintf(fic, "</CellData>\n");
    fprintf(fic, "</Piece>\n");
    fprintf(fic, "</StructuredGrid>\n");
    fprintf(fic, "</VTKFile>\n");
    fclose(fic);
}
