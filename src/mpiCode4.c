#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"
#include <time.h>



int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*Seed each process individually*/
    srandom(time(NULL) + rank);

    debug(rank, "hello (p=%d)\n", num_procs);
    if (rank == 0){
        send_to_many(num_procs);
    } else {
        receive_from_one(rank);
    }

    debug(rank, "goodbye\n");
    MPI_Finalize();
    return 0;
}