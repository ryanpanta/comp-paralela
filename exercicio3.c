// Author: Wes Kendall
// Copyright 2012 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Program that computes the minimum value of an array of elements in parallel
// using MPI_Scatter and MPI_Gather

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Function to create an array of random numbers between 0 and 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  for (int i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Function to compute the minimum value in an array of numbers
float compute_min(float *array, int num_elements) {
  float min = array[0];
  for (int i = 1; i < num_elements; i++) {
    if (array[i] < min) {
      min = array[i];
    }
  }
  return min;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: min num_elements_per_proc\n");
    exit(1);
  }

  int num_elements_per_proc = atoi(argv[1]);
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
  }

  float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  float sub_min = compute_min(sub_rand_nums, num_elements_per_proc);

  float *sub_mins = NULL;
  if (world_rank == 0) {
    sub_mins = (float *)malloc(sizeof(float) * world_size);
    assert(sub_mins != NULL);
  }

  MPI_Gather(&sub_min, 1, MPI_FLOAT, sub_mins, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    float min = compute_min(sub_mins, world_size);
    printf("Minimum value is %f\n", min);
  }

  if (world_rank == 0) {
    free(rand_nums);
    free(sub_mins);
  }

  free(sub_rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  return 0;
}
