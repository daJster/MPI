# MPI
This repo is for MPI and OpenMP courses and practice.

**MPI FUNCTIONS**

```c
/*requirements :*/
#include <mpi/mpi.h>
// running on mpicc and enabled lamboot

/*Set up*/
int MPI_Init(int *argc, char ***argv);

/*Tear down*/
int MPI_Finalize();

/*Total processes*/
int MPI_Comm_size(MPI_Comm comm, int *size); // filling size

/*Local process index*/
int MPI_Comm_rank(MPI_Comm comm, int *rank); // filling rank

/*Primitive communication*/
/*Send*/
int MPI_Send(void * buf,
    int count, 
    MPI_Datatype datatype,
    int dest, 
    int tag, 
    MPI_Comm comm);

/*Receive*/
int MPI_Recv(void * buf,
    int count, 
    MPI_Datatype datatype,
    int src, 
    int tag, 
    MPI_Comm comm,
    MPI_Status *status);



/*MPI Status Value*/
typedef struct MPI_Status{
    int MPI_SOURCE;
    int MPI_TAGE;
    int MPI_ERROR;
} MPI_status;


/*MPI DATATYPES*/
MPI_SIGNED_CHAR / MPI_UNSIGNED_CHAR
MPI_SHORT / MPI_UNSIGNED_SHORT
MPI_INT / MPI_UNSIGNED
MPI_LONG / MPI_UNSIGNED_LONG
MPI_LONG_LONG_INT (a.k.a MPI_LONG_LONG) / MPI_UNSIGNED_LONG_LONG
MPI_CHAR
MPI_WCHAR
MPI_FLOAT
MPI_DOUBLE
MPI_LONG_DOUBLE

MPI_INT8_T / MPI_UINT8_T
MPI_INT16_T / MPI_UINT16_T
MPI_INT32_T / MPI_UINT32_T
MPI_INT64_T / MPI_UINT64_T

MPI_C_BOOL
MPI_C_COMPLEX
MPI_C_FLOAT_COMPLEX
MPI_C_DOUBLE_COMPLEX
MPI_C_LONG_DOUBLE_COMPLEX

MPI_AINT
MPI_COUNT
MPI_OFFSET
MPI_BYTE
MPI_PACKED

// List of datatypes for reduction functions MPI_MINLOC and MPI_MAXLOC:
MPI_SHORT_INT
MPI_LONG_INT
MPI_FLOAT_INT
MPI_DOUBLE_INT
MPI_LONG_DOUBLE_INT
MPI_2INT

```
