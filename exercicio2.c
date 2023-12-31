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

    MPI_Init(NULL, NULL);
    // Get the number of processes
    int world_size;.
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
/*   if(world_size != 2) {
        fprintf(stderr, "Must use two processes for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
  } */
  
  // Get the rank of the process
    int world_rank;
    int soma = 0;
    int somatotal = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int MAX_NUMBERS = 2000;
    int numbers[MAX_NUMBERS];
    int number_amount;
    MPI_Status status;
    int tag;

    if(world_rank == 0) {

        srand(time(NULL));

        for(int m = 1; m < world_size; m++){

            for(int i = 0; i < number_amount; i++){
            numbers[i] = rand() % 100;
            }


            MPI_Send(numbers, number_amount, MPI_INT, m, 0, MPI_COMM_WORLD);
            printf("0 sent %d numbers to %d\n", number_amount, m);
            soma = 0;
        }
        for(int m = 1; m < world_size; m++){
            MPI_Recv(&soma, 1, MPI_INT, m, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            somatotal += soma;
        }
        printf("A soma total foi de: %d \n", somatotal);

    } else {
        MPI_Status status;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        tag = status.MPI_TAG;

        if (tag == 0){

        }
        else if(tag == 1) {

        }
        else if(tag == 2) {

        }
        else if(tag == 3) {

        }
        else if(tag == 10) {
            
        }
        MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &number_amount);
        printf("%d received %d numbers from 0. Message source = %d, tag = %d\n", world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);
        for(int i = 0; i < number_amount; i++)
            soma += numbers[i];
        MPI_Send(&soma, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("%d somou: %d  \n", world_rank, soma);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    }

    /* 1° Faça um programa com a estrutura mestre/escravo, onde: 

    - mestre: gera uma quantidade aleatória entre 1000 e 2000 números de valores aleatórios entre 0 e 99, e envia para escravo.

    - escravo: recebe os valores gerados pelo mestre, calcula a somatória dos valores e devolve para o mestre.

    */
