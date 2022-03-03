
#include <stdio.h>
#include <stdlib.h>

#include "../spryng.h"

int main(){

    printf("/--abc--def--ghi--jkl/\n");

    char** tk = splits("--abc--def--ghi--jkl", "--");

    for (int i=0; i<4; i++){

        println("[%s]", tk[i]);
    }

    return 0;
}
