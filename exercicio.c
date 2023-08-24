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
  int soma = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 2000;
  int numbers[MAX_NUMBERS];
  int number_amount;
  if(world_rank == 0) {
    srand(time(NULL));
    number_amount = 1000 + rand() % (MAX_NUMBERS - 1000 + 1);
    for(int i = 0; i < number_amount; i++){
        numbers[i] = rand() % 100;
    }
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to %d\n", number_amount, 1);

  } else {
    MPI_Status status;
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &number_amount);
    printf("%d received %d numbers from 0. Message source = %d, tag = %d\n", world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);
    for(int i = 0; i < number_amount; i++)
        soma += numbers[i];
    printf("Soma: %d ", soma);

  }

  MPI_Barrier(MPI_COMM_WORLD);

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();

}

/* 1° Faça um programa com a estrutura mestre/escravo, onde: 

- mestre: gera uma quantidade aleatória entre 1000 e 2000 números de valores aleatórios entre 0 e 99, e envia para escravo.

- escravo: recebe os valores gerados pelo mestre, calcula a somatória dos valores e devolve para o mestre.

*/
