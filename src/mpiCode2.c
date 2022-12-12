#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <time.h>

/*Round robin program*/

void round_robin(int rank, int num_procs){
    long int rand_mine, rand_prev;
    int rank_next = (rank + 1)%num_procs;
    int rank_prev = rank == 0 ? num_procs - 1: rank - 1; // torus
    MPI_Status status;

    srandom(time(NULL) + rank);
    rand_mine = random() / (RAND_MAX / 100);
    printf("[%d] : random is %ld\n", rank, rand_mine);

    if (rank % 2 == 0){
        // sending a random number (int long) to the next processor
        printf("[%d] : sending %ld to %d\n", rank, rand_mine, rank_next);
        MPI_Send((void *)&rand_mine, 1, MPI_LONG, rank_next, 1, MPI_COMM_WORLD);

        // receiving a random number MPI_LONG from the previous processor
        printf("[%d] : receiving from %d\n", rank, rank_prev);
        MPI_Recv((void *)&rand_prev, 1, MPI_LONG, rank_prev, 1, MPI_COMM_WORLD, &status);
    } else {
        // receiving first then sending
        printf("[%d] : receiving from %d\n", rank, rank_prev);
        MPI_Recv((void *)&rand_prev, 1, MPI_LONG, rank_prev, 1, MPI_COMM_WORLD, &status);

        printf("[%d] : sending %ld to %d\n", rank, rand_mine, rank_next);
        MPI_Send((void *)&rand_mine, 1, MPI_LONG, rank_next, 1, MPI_COMM_WORLD);
    }

    printf("[%d] : >> I had %ld, %d had %ld\n", rank, rand_mine, rank_next, rand_prev);
}

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