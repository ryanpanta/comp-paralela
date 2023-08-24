#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv) {
  const int PING_PONG_LIMIT = 10;
  MPI_Init(NULL, NULL);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if (world_size != 2) {
    fprintf(stderr, "Must use two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 2000;
  int numbers[MAX_NUMBERS];
  int number_amount;
  int soma = 0; // Initialize soma

  if (world_rank == 0) {
    srand(time(NULL));
    number_amount = 1000 + rand() % (MAX_NUMBERS - 1000 + 1);
    for (int i = 0; i < number_amount; i++) {
      numbers[i] = rand() % 100;
    }

    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", number_amount);

  } else if (world_rank == 1) {
    MPI_Status status;
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &number_amount);
    printf("1 received %d numbers from 0.\n", number_amount);

    for (int i = 0; i < number_amount; i++) {
      soma += numbers[i];
    }
    printf("Soma: %d\n", soma);

  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
