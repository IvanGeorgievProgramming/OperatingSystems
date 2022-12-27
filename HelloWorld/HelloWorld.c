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