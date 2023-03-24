#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int id = fork();

    if (id == 0){
        sleep(1);
    }

    printf("Current ID: %d, Parent ID: %d\n", getpid(), getppid());

    int res = wait(NULL);
    if(res == -1){
        printf("No child process to wait for\n");
    }
    else{
        printf("Child process with ID %d has terminated\n", res);
    }

    return 0;
}