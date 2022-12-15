#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"


//Shout it out to the whole world TD3

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (num_procs < 2){
        debug(rank, "This application must be run with at least 2 MPI processes, not %d\n", num_procs);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    MPI_Status status;

    if (rank == 0){
        int snd_value = 1239;
        debug(rank, "Broadcasting value : %d\n", snd_value);
        MPI_Bcast(&snd_value, 1, MPI_INT, rank, MPI_COMM_WORLD);
    } else {
        int rcv_value = 0;
        MPI_Bcast(&rcv_value, 1, MPI_INT, 0, MPI_COMM_WORLD);        
        debug(rank, "receiving value : %d\n", rcv_value);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}