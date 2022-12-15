#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"


// An incorrect hello world TD4 find the bug

int main(int argc, char **argv){

    MPI_Init(&argc, &argv);

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
	int comm_size;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
 
	printf("\"Hello World!\" from MPI process %d. We are %d MPI processes.\n", my_rank, comm_size);
 
	MPI_Finalize();
    return 0;
}