#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"

// Distributed sum/prod TD5

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    long long int value = 1 + rank*100;
    debug(rank, "Value held is : %d\n", value);
    MPI_Barrier(MPI_COMM_WORLD);


    // perform the reduction
    long long int sum;
    MPI_Reduce(&value, &sum, 1, MPI_LONG_LONG_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    if (rank == 0){
        debug(rank, "Total sum is : %d\n", sum);
    }
    MPI_Finalize();
    return 0;
}