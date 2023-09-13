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

  float *rand_nums1 = NULL;
  float *rand_nums2 = NULL;
  if (world_rank == 0) {
    rand_nums1 = create_rand_nums(num_elements_per_proc * world_size);
    rand_nums2 = create_rand_nums(num_elements_per_proc * world_size);
  }

  float *sub_rand_nums1 = (float *)malloc(sizeof(float) * num_elements_per_proc);
  float *sub_rand_nums2 = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums1 != NULL);
  assert(sub_rand_nums2 != NULL);

  MPI_Scatter(rand_nums1, num_elements_per_proc, MPI_FLOAT, sub_rand_nums1,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(rand_nums2, num_elements_per_proc, MPI_FLOAT, sub_rand_nums2,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Calculate the sum of the submatrices
  float *sub_result = (float *)malloc(sizeof(float) * num_elements_per_proc);
  for (int i = 0; i < num_elements_per_proc; i++) {
    sub_result[i] = sub_rand_nums1[i] + sub_rand_nums2[i];
  }

  float *result = NULL;
  if (world_rank == 0) {
    result = (float *)malloc(sizeof(float) * num_elements_per_proc * world_size);
    assert(result != NULL);
  }

  MPI_Gather(sub_result, num_elements_per_proc, MPI_FLOAT, result,
             num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    printf("Resultant Matrix:\n");
    for (int i = 0; i < num_elements_per_proc * world_size; i++) {
      printf("%f ", result[i]);
    }
    printf("\n");
    free(rand_nums1);
    free(rand_nums2);
    free(result);
  }

  free(sub_rand_nums1);
  free(sub_rand_nums2);
  free(sub_result);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  return 0;
}
