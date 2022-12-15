#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);

    int ndims = 2;
    int dims[2] = {0, 1};
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    // Ask MPI to decompose our processes in a 2D cartesian grid for us
    MPI_Dims_create(num_procs, ndims, dims);

    int periods[2] = {1, 0};
    MPI_Comm comm_cart;

    MPI_Cart_create(MPI_COMM_WORLD, // Input Communicator
                ndims, // Number of cartesian dimensions
                dims, // Processes per dimesion
                periods, // Periodicity per dimension
                0, // Reorder ranking
                &comm_cart); // Cartesian communicator

    MPI_Comm_rank(comm_cart, &rank);
    int coords[2];

    MPI_Cart_coords(comm_cart, rank, ndims, coords);
    debug(rank, "coords [ %d ; %d ] \n", coords[0], coords[1]);

    MPI_Finalize();
    return 0;
}