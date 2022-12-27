#include <stdio.h>
#include <stdlib.h>

int main(){
    char *name = malloc(20 * sizeof(char));

    printf("Enter your name: ");
    scanf("%s", name);
    printf("Hello %s", name);

    free(name);

    return 0;
}

/*
Linux - Compile and run:

gcc HelloWorld/HelloWorld.c
./a.out
*/