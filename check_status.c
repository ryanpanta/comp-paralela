// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv) {
  const int PING_PONG_LIMIT = 10;
  // Initialize the MPI environment. The two arguments to MPI Init are not
  // currently used by MPI implementations, but are there in case future
  // implementations might need the arguments.
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if(world_size != 2) {
        fprintf(stderr, "Must use two processes for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
  }
  

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 100;
  int numbers[MAX_NUMBERS];
  int number_amount;
  if(world_rank == 0) {
    srand(time(NULL));
    number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", number_amount);
  } else if (world_rank == 1) {
    MPI_Status status;
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &number_amount);
    printf("1 received %d numbers from 0. Message source = %d, tag = %d\n", number_amount, status.MPI_SOURCE, status.MPI_TAG);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}
