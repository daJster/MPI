#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"



int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d: Hello (p=%d)\n", rank, num_procs);
    /* Do many things, all at once*/
    MPI_Finalize();
    return 0;
}