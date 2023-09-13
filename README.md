# Comandos para compilação e execução do arquivo MPI


## Padrão 

`mpicc exercicio.c -o exercicio`  
`mpirun -np 5 --hostfile hostfile ./exercicio`  

## bcast

`mpicc 06_compare_bcast.c -o 06_compare_bcast`  
mpirun -np 10 --hostfile hostfile ./06_compare_bcast 2000 50

## no windows, compila assim:

gcc -fdiagnostics-color=always -g exercicio4.c -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi -o exercicio4.exe

** e executa assim: **

`mpiexec -n 10 exercicio4.exe 10`