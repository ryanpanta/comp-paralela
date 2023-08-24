#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main () {
    const int MAX_NUMBERS = 2000;
    int numbers[MAX_NUMBERS];
    int number_amount;

    srand(time(NULL)); 

    number_amount = 1000 + rand() % (MAX_NUMBERS - 1000 + 1);
    for(int i = 0; i < number_amount; i++){
        printf("%d ", rand() % 100);
    }
}