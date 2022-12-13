#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <time.h>
#include "utils.h"


/*Round robin program*/

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    printf("[%d] : hello (p=%d)\n", rank, num_procs);
    round_robin(rank, num_procs);

    printf("[%d] : goodbye\n", rank);
    MPI_Finalize();
    return 0;
}