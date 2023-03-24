#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arrSize = sizeof(arr) / sizeof(int);
    int start, end;
    int fd[2];

    if(pipe(fd) == -1){
        printf("Error creating pipe");
        return 1;
    }

    int id1 = fork();
    int id2;
    if(id1 == -1){
        printf("Error creating child process");
        return 1;
    }

    if(id1 == 0){
        // Child 1
        id2 = fork();
        if(id2 == -1){
            printf("Error creating child process");
            return 1;
        }
    }

    if(id1 == 0 && id2 == 0){
        // Child 2
        start = 0;
        end = arrSize / 3;
    }
    else if(id1 == 0 && id2 != 0){
        // Child 1
        start = arrSize / 3;
        end = arrSize / 3 * 2;
    }
    else{
        // Parent
        start = arrSize / 3 * 2;
        end = arrSize;
    }

    int sum = 0;

    for(int i = start; i < end; i++){
        sum += arr[i];
    }

    printf("Calculated partial sum: %d", sum);

    return 0;
}