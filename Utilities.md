
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


/*Simultaneaous Send and Receive*/
int MPI_Sendrecv(void *sendbuf,
                int send_count,
                MPI_Datatype datatype,
                int dest,
                int sendtag,
                
                
                void *recvbuf,
                int recv_count,
                MPI_Datatype datatype,
                int source,
                int recvtag,

                MPI_Comm comm,
                MPI_Status *status);


/*Barrier and Broadcast*/
/*Return after every process calls */
int MPI_Barrier(MPI_Comm);

/*One-to-all broadcast*/
int MPI_Bcast(void *buf,
            int count,
            MPI_Datatype datatype,
            int source,
            MPI_Comm comm);


/*Non-blocking communication*/
/*Non_blocking Send*/
int MPI_Isend(void *buf,
            int count,
            MPI_Datatype datatype,
            int dest,
            int tag,
            MPI_Comm comm,
            MPI_REQUEST *request); // information about the progress of the send

/*Non_blocking Receive*/
int MPI_Irecv(void *buf,
            int count,
            MPI_Datatype datatype,
            int source,
            int tag,
            MPI_Comm comm,
            MPI_REQUEST *request);

/*Await Non-Blocking completion*/
/*Test whether request complete*/
// it's a Non-Blocking test
int MPI_Test(MPI_REQUEST *request,
            int *flag,
            MPI_Status *status);

/*Wait for request to complete*/
// waits for the request to be filled, either with a success or a failure
int MPI_Wait(MPI_REQUEST * request,
            MPI_STATUS *status);

/*MPI Status Value*/
typedef struct MPI_Status{
    int MPI_SOURCE;
    int MPI_TAGE;
    int MPI_ERROR;
} MPI_Status;

/*MPI Request Value*/ // not sure !
typedef struct MPI_Request{
    int MPI_SOURCE;
    int MPI_TAGE;
    int MPI_ERROR;
} MPI_Request;


/*MPI TOPOLOGIES*/
/*Create Cartesian topology*/
int MPI_Cart_create(MPI_Comm comm_old,
                    int ndims,
                    int *dims,
                    int *periods, // torus, true or false value
                    int reorder,
                    MPI_Comm *comm_cart);

/*Coordinates to rank*/
int MPI_Cart_rank(MPI_Comm comm_cart,
                int *coords,
                int *rank);

/*Rank to coordinates*/
int MPI_Cart_coords(MPI_Comm comm_cart,
                int rank,
                int maxdims,
                int *coords);

/*Shift along a dimension*/
int MPI_Cart_shift(MPI_Comm comm_cart,
                    int dir, // dimesnion of topology
                    int s_step, // step size
                    int *rank_source,
                    int *rank_dest);



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
