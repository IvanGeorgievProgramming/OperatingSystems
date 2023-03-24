#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    int fd[2];
    // fd[0] - read
    // fd[1] - write
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
        close(fd[0]);
        int n;
        printf("Enter a number: ");
        scanf("%d", &n);
        if(write(fd[1], &n, sizeof(int)) == -1){
            printf("Error writing to pipe\n");
            return 1;
        }
        close(fd[1]);
    }
    else{
        wait(NULL);
        close(fd[1]);
        int r;
        if(read(fd[0], &r, sizeof(int)) == -1){
            printf("Error reading from pipe\n");
            return 1;
        }
        close(fd[0]);
        printf("Got from child process: %d\n", r);
    }

    return 0;
}