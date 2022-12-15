#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"

#define ROWS 4
#define COLS 3
// Take as much as u need TD7

int main(int argc, char **argv){
    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (num_procs != 3){
        debug(rank, "This application must run on 3 processes only!, you used %d", num_procs);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    if (rank == 0){
        // Initialise the array
		int entire_array[ROWS][COLS];
		for(int i = 0; i < ROWS; i++)
		{
			for(int j = 0; j < COLS; j++)
			{
				entire_array[i][j] = i * COLS + j;
				printf("%2d", entire_array[i][j]);
			}
			printf("\n");
		}
		printf("\n");

        // Create a vector extracting an integer every two integers
		MPI_Datatype BRICK;
		MPI_Type_vector(ROWS * COLS / 2, 1, 2, MPI_INT, &BRICK);
    } else {}


    MPI_Finalize();
    return 0;
}