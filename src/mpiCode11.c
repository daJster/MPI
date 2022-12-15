#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"

// non-blocking send receive TD6 switch to non-blocking

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (num_procs != 2 ){
        debug(rank, "This application must run on only 2 MPI processes, instead of %d\n", num_procs);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    if (rank == 0){
        MPI_Request request;
        MPI_Status status;
        int snd_value = 42069;
        debug(rank, "Sending with non-blocking send value : %d\n", snd_value);
        MPI_Isend(&snd_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

    } else {
        MPI_Request request;
        int rcv_value = -1;
        MPI_Irecv(&rcv_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        debug(rank, "Receiving with non-blocking recv value : %d\n", rcv_value);

        MPI_Wait(&request, MPI_STATUS_IGNORE);
        debug(rank, "The wait completed, and I received %d\n", rcv_value);
    }

    MPI_Finalize();
    return 0;
}