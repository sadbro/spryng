
#include <stdio.h>
#include <stdlib.h>

#include "../spryng.h"

int main(){

    char delim;
    char buff[64];
    char** list;

    printf("Enter delimiter: ");
    scanf("%c", &delim);

    printf("\nEnter Script (max size of 64): ");
    scanf("%s", buff);

    println("-------------------------------");

    list = split(buff, delim);

    for (int i=0; i<count(buff, delim) +1; i++){

        println("[%s]", list[i]);
    }

	return 0;
}
