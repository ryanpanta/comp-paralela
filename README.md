# Comandos para compilação e execução do arquivo MPI


## Padrão 

`mpicc exercicio.c -o exercicio`  
`mpirun -np 5 --hostfile hostfile ./exercicio`  

## bcast

`mpicc 06_compare_bcast.c -o 06_compare_bcast`  
mpirun -np 10 --hostfile hostfile ./06_compare_bcast 2000 50