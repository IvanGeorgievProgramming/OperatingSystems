#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// ! Wait(NULL) returns -1 if there is no child process to wait for

/*

    -------------
    |  id1 = x  |---------------
    |  id2 = z  |              |
    -------------              |
          |                    |
          V                    V
    -------------        -------------
  X |  id1 = 0  |      Z |  id1 = x  |  
    |  id2 = y  |        |  id2 = 0  |
    -------------        -------------
          |
          V
    -------------
  Y |  id1 = 0  |
    |  id2 = 0  |
    -------------

*/

int main(int argc, char *argv[]){
    int id1 = fork();
    int id2 = fork();

    while(wait(NULL) != -1 || errno != ECHILD){
        printf("Waited for a child to finish\n");
    }

    if (id1 == 0){
        if(id2 == 0){
            printf("I am the child of the first child\n");
        }
        else{
            printf("I am the first child of the parent\n");
        }
    }
    else{
        if(id2 == 0){
            printf("I am the second child of the parent\n");
        }
        else{
            printf("I am the parent\n");
        }
    }

    return 0;
}