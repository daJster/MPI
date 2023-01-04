#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

#define N 5
#define M 5
#define K 5

int main(int argc, char *argv[])
{
    int i, j, k;
    double *A, *B, *C;
    int my_rank, num_procs;
    int rows_per_proc;
    int offset;
    MPI_Status status;


    /* Allocate memory for the matrices */
    A = (double *)malloc(N * M * sizeof(double));
    B = (double *)malloc(M * K * sizeof(double));
    C = (double *)malloc(N * K * sizeof(double));

    /* Initialize the matrices */
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            A[i * M + j] = 1.0;
        }
    }
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            B[i * K + j] = 1.0;
        }
    }

     for (i = 0; i < N; i++) {
        for (j = 0; j < K; j++) {
            C[i * K + j] = 0.0;
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    /* Check that the matrices are compatible for multiplication */
    if (M != K) {
        if (my_rank == 0) {
        printf("Error: incompatible matrices\n");
        }
        MPI_Finalize();
        return 0;
    }

    /* Divide the rows of A evenly among the processes */
    rows_per_proc = N / num_procs;
    offset = my_rank * rows_per_proc;

    /* Compute the product C = A * B */
    for (i = offset; i < offset + rows_per_proc; i++) {
        for (j = 0; j < K; j++) {
            for (k = 0; k < M; k++) {
                C[i * K + j] = C[i * K + j] + A[i * M + k] * B[k * K + j];
            } 
        }
    }

    /* Gather the partial results from each process */
    if (my_rank == 0) {
        double *temp = (double *)malloc(N * K * sizeof(double));
        for (i = 1; i < num_procs; i++) {
            offset = i * rows_per_proc;
            MPI_Recv(temp, rows_per_proc, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            for (j = 0; j < rows_per_proc; j++) {
                for (k = 0; k < K; k++) {
                    C[(offset + j) * K + k] = temp[j * K + k];
                }
            }
        }
        free(temp);
    } else {
        MPI_Send(C + offset, rows_per_proc, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0){
        /*Printing the result matrix*/
        for( i = 0; i < N; i++){
            for (j = 0; j < K; j++){
                printf("%f ", C[ i * K + j]);
            }
            printf("\n");
        }
    }
    

    MPI_Finalize();
    free(A);
    free(B);
    free(C);
    return 0;
}