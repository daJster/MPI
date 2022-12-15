#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"

// Ordered Hello world TD8 (VERY IMMPORTANT TO UNDERSTAND)

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int i = 0;

    while (i < rank){
        MPI_Barrier(MPI_COMM_WORLD);
        i++;
    }

    debug(rank, "\"Hello World!\"\n");

    while(i < num_procs){
        MPI_Barrier(MPI_COMM_WORLD);
        i++;
    }

    MPI_Finalize();
    return 0;
}