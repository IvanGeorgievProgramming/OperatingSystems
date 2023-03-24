#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    int arr[] = {1, 2, 3, 4, 1, 2, 7};
    int arrSize = sizeof(arr) / sizeof(int);
    int start, end;
    int fd[2];

    if(pipe(fd) == -1){
        printf("Error creating pipe\n");
        return 1;
    }

    int id = fork();
    if(id == -1){
        printf("Error creating child process\n");
        return 1;
    }
    
    if(id == 0){
        // Child
        start = 0;
        end = arrSize / 2;
    }
    else{
        // Parent
        start = arrSize / 2;
        end = arrSize;
    }

    int sum = 0;
    for(int i = start; i < end; i++){
        sum += arr[i];
    }

    printf("Calculated partial sum: %d\n", sum);

    if(id == 0){
        // Child
        close(fd[0]);
        if(write(fd[1], &sum, sizeof(sum))){
            printf("Error writing to pipe\n");
            return 1;
        }
        close(fd[1]);
    }
    else{
        close(fd[1]);
        int sumFromChild;
        if(read(fd[0], &sumFromChild, sizeof(sumFromChild))){
            printf("Error reading from pipe\n");
            return 1;
        }
        close(fd[0]);

        int totalSum = sum + sumFromChild;
        printf("Total sum: %d\n", totalSum);
        wait(NULL);
    }

    return 0;
}