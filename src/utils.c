#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <time.h>
#include <stdarg.h> 
#include <unistd.h>
#include "utils.h"

#define BUSY_WAIT 10 // to comment/uncomment


void debug(int rank, char *format, ...){

    va_list args; /*Variable argument list*/

    /*  Initialize variable argument list;
    `format` is last argument before ...
    */
    time_t t = time(NULL);  
    struct tm * timeinfo;
    timeinfo =  localtime(&t);

    va_start(args, format);

    printf("%d:%d:%d |%2d|", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, rank);
    vprintf(format, args);

    /*   Clean up variable argument list*/
    va_end(args);
}

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

        // Could use simultaneous send receive
        //
        // MPI_Sendrecv( (void *) &rand_mine, 1, MPI_LONG, rank_next, 1, // sending
        //              (void *) &rand_prev, 1, MPI_LONG, rank_prev, 1, // receiving
        //              MPI_COMM_WORLD, &status); 
        //
    } else {
        // receiving first then sending
        printf("[%d] : receiving from %d\n", rank, rank_prev);
        MPI_Recv((void *)&rand_prev, 1, MPI_LONG, rank_prev, 1, MPI_COMM_WORLD, &status);

        printf("[%d] : sending %ld to %d\n", rank, rand_mine, rank_next);
        MPI_Send((void *)&rand_mine, 1, MPI_LONG, rank_next, 1, MPI_COMM_WORLD);
    }

    printf("[%d] : >> I had %ld, %d had %ld\n", rank, rand_mine, rank_next, rand_prev);
}

long broadcast(int rank, int num_procs){
    long random_value;
    int broadcaster_rank = num_procs - 1; // choosing broadcaster

    if (rank == broadcaster_rank){
        // Setting random value
        srandom(time(NULL) + rank);
        random_value = random() / (RAND_MAX / 10);
        debug(rank, "broadcasting %ld\n", random_value);
    }

    // Broadcasting
    // note : doing it here or in the first if is the same, because broadcaster_rank (source), is the same.
    MPI_Bcast((void *) &random_value, 1, MPI_LONG, broadcaster_rank, MPI_COMM_WORLD);


    if (rank != broadcaster_rank){
        // receiving the random value
        debug(rank, "received %ld\n", random_value);
    }

    return random_value;
}


void barrier(int rank, long random_value){
    int nap_time = random_value + (2 * rank);
    debug(rank, "sleeping %ds\n", nap_time);
    sleep(nap_time);

    debug(rank, "Enter Barrier\n");
    MPI_Barrier(MPI_COMM_WORLD);
    debug(rank, "Leave Barrier\n");
}


void send_to_many(int num_procs){
    long random_value = random() / (RAND_MAX / 100);
    debug(0, "random value is : %ld\n", random_value);
    MPI_Request send_request;

    for (int to_rank = 1; to_rank < num_procs; to_rank++){
        long sent_value = random_value + to_rank;

        MPI_Isend((void *) &sent_value, 1, MPI_LONG, to_rank, 1, MPI_COMM_WORLD, &send_request);

        await_request(0, &send_request);
        debug(0, "sent %ld to %d\n", sent_value, to_rank);
    }
}

void receive_from_one(int rank){
    long recv_buf;
    MPI_Request recv_request;

    MPI_Irecv((void *) &recv_buf, 1, MPI_LONG, 0, 1, MPI_COMM_WORLD, &recv_request);

    await_request(rank, &recv_request);
    debug(rank, "received %ld\n", recv_buf);
}

void await_request(int rank, MPI_Request *request){
    // testing the request while waiting for the send/recv to finish
    #ifdef BUSY_WAIT
        int wait_count = 0;
        int flag = 0;
        MPI_Status status;

        do{
            wait_count++;
            MPI_Test(request, &flag, &status);
        } while (!flag);
        debug(rank, "tested %d times\n", wait_count);
    #endif
}
