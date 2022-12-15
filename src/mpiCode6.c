#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"


// Hello world TD 1

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("\"Hello World!\" from MPI process %d, we are %d MPI processes.\n", rank, num_procs);

    MPI_Finalize();
    return EXIT_SUCCESS;
}