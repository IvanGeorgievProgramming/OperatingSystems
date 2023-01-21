#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<err.h>
#include<sys/wait.h>

int numRows(int file);
int* sizeRows(int file);
char** getRows(int file);
int numWords(char* row);
int* sizeWords(char* row);
char** getWords(char* row);
void executeCommands(char* filename);

int main(int argc, char *argv[]){
    executeCommands("CommandInterpreter/commands");
}

int numRows(int file){
    lseek(file, 0, SEEK_SET);

    int lines = 0, bytes = 0;
    char symbol;
    int readStatus = read(file, &symbol, 1);

    while(readStatus == 1){
        bytes++;
        if(symbol == '\n'){
            lines++;
            bytes++;
        }
        readStatus = read(file, &symbol, 1);
    }

    lines++;

    return lines;
}

int* sizeRows(int file){
    int numOfRows = numRows(file);

    int* sizeOfRows = (int*)malloc(numOfRows * sizeof(int));

    int rowIndex = 0;
    int charIndex = 0;

    lseek(file, 0, SEEK_SET);

    char symbol;
    int readStatus = read(file, &symbol, 1);

    while(readStatus == 1){
        if(symbol == '\n'){
            sizeOfRows[rowIndex] = charIndex;
            rowIndex++;
            charIndex = 0;
        } else {
            charIndex++;
        }
        readStatus = read(file, &symbol, 1);
    }

    return sizeOfRows;
}

char** getRows(int file){
    int numOfRows = numRows(file);

    char** rows = (char**)malloc((numOfRows + 1) * sizeof(char*));
    rows[numOfRows] = NULL;

    int* sizeOfRows = sizeRows(file);

    for(int i = 0; i < numOfRows; i++){
        rows[i] = (char*)malloc((sizeOfRows[i] + 1) * sizeof(char));
        rows[i][sizeOfRows[i]] = '\0';
    }

    int rowIndex = 0;
    int charIndex = 0;

    lseek(file, 0, SEEK_SET);

    char symbol;
    int readStatus = read(file, &symbol, 1);

    while(readStatus == 1){
        if(symbol == '\n'){
            rowIndex++;
            charIndex = 0;
        } else {
            rows[rowIndex][charIndex] = symbol;
            charIndex++;
        }
        readStatus = read(file, &symbol, 1);
    }

    return rows;
}

int numWords(char* row){
    int numOfWords = 0;

    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            numOfWords++;
        }
    }

    numOfWords++;

    return numOfWords;
}

int* sizeWords(char* row){
    int numOfWords = numWords(row);

    int* sizeOfWords = (int*)malloc(numOfWords * sizeof(int));

    int wordSize = 0;
    int wordIndex = 0;

    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            sizeOfWords[wordIndex] = wordSize;
            wordIndex++;
            wordSize = 0;
        } else {
            wordSize++;
        }
    }

    sizeOfWords[wordIndex] = wordSize;

    return sizeOfWords;
}

char** getWords(char* row){
    int numOfWords = numWords(row);

    char** words = (char**)malloc((numOfWords + 1) * sizeof(char*));
    words[numOfWords] = NULL;

    int* sizeOfWords = sizeWords(row);

    for(int i = 0; i < numOfWords; i++){
        words[i] = (char*)malloc((sizeOfWords[i] + 1) * sizeof(char));
        words[i][sizeOfWords[i]] = '\0';
    }

    int wordIndex = 0;
    int charIndex = 0;

    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            wordIndex++;
            charIndex = 0;
        } else {
            words[wordIndex][charIndex] = row[i];
            charIndex++;
        }
    }

    return words;
}

void executeCommands(char* filename){
    int fd = open(filename, O_RDONLY);
    if(fd == -1){
        err(1, "Error opening file");
    }

    int numOfRows = numRows(fd);
    char** rows = getRows(fd);

    for(int i = 0; i < numOfRows; i++){
        char** words = getWords(rows[i]);
        
        int childPid = fork();

        if(childPid == -1){
            err(1, "Error forking");
        }
        else if(childPid == 0){
            if(execvp(words[0], words) == -1){
                err(1, "Error executing command");
            }
        }
        else{
            int status;
            int child = wait(&status);

            if(WIFEXITED(status)){
                printf("Child %d exited with status %d\n", child, WEXITSTATUS(status));
            }
        }
    }

    close(fd);
}