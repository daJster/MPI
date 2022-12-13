#ifndef __UTILS__H
#define __UTILS__H

void debug(int rank, char *format, ...);

// mpiCode2
void round_robin(int rank, int num_procs);



// mpiCode3
long broadcast(int rank, int num_procs);
void barrier(int rank, long random_value);

// mpiCode4
void send_to_many(int num_procs);
void receive_from_one(int rank);
void await_request(int rank, MPI_Request *request);


#endif // __UTILS__H