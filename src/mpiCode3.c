#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi/mpi.h>
#include "utils.h"



int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    debug(rank, "hello (p=%d)\n", num_procs);
    long random_value = broadcast(rank, num_procs);
    barrier(rank, random_value);
    debug(rank, "goodbye\n");

    MPI_Finalize();
    return 0;
}