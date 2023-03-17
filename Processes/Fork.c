#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void howToDetectIfProcessIsChildOrParent();
void howManyTimesWillThisPrint();
void howToPrint3Times();

int main(int argc, char *argv[]){

    howToDetectIfProcessIsChildOrParent();

    return 0;
}

void howToDetectIfProcessIsChildOrParent(){
    int id = fork();

    if(id == 0){
        printf("I am the child process\n");
    }
    else{
        printf("I am the parent process\n");
    }
}

void howManyTimesWillThisPrint(){
    // 2^3 = 8
    fork();
    fork();
    fork();
    printf("Hello World\n");
}

void howToPrint3Times(){
    int id = fork();

    if(id != 0){
        fork();
    }

    printf("Hello World!\n");
}