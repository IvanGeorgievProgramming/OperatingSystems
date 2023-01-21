#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <err.h>
#include <fcntl.h>
#include <time.h>

int numRows(int file, int* errorDetected);
int* sizeRows(int file, int* errorDetected);
char** getRows(int file, int* errorDetected);
int numWords(char* row, int* errorDetected);
int* sizeWords(char* row, int* errorDetected);
char** getWords(char* row, int* errorDetected);
void errorHandler(int errorDetected);
void executeCommands(char* filename);

int main(int argc, char *argv[]){
    if(argc != 2){
        errx(1, "Invalid number of arguments");
    }
    
    executeCommands(argv[1]);
}

int numRows(int file, int* errorDetected){
    if(lseek(file, 0, SEEK_SET) == -1){
        errorDetected = -1;
        return;
    }

    int lines = 0;
    char symbol;
    int readStatus = read(file, &symbol, 1);
    if(readStatus == -1){
        errorDetected = -2;
        return;
    }

    while(readStatus == 1){
        if(symbol == '\n'){
            lines++;
        }
        readStatus = read(file, &symbol, 1);
        if(readStatus == -1){
            errorDetected = -2;
            return;
        }
    }

    lines++;

    return lines;
}

int* sizeRows(int file, int* errorDetected){
    int numOfRows = numRows(file, errorDetected);

    int* sizeOfRows = (int*)malloc(numOfRows * sizeof(int));
    if(sizeOfRows == NULL){
        errorDetected = -3;
        return;
    }

    int rowIndex = 0;
    int charIndex = 0;

    if(lseek(file, 0, SEEK_SET) == -1){
        errorDetected = -1;
        return;
    }

    char symbol;
    int readStatus = read(file, &symbol, 1);
    if(readStatus == -1){
        errorDetected = -2;
        return;
    }

    while(readStatus == 1){
        if(symbol == '\n'){
            sizeOfRows[rowIndex] = charIndex;
            rowIndex++;
            charIndex = 0;
        } else {
            charIndex++;
        }
        readStatus = read(file, &symbol, 1);
        if(readStatus == -1){
            errorDetected = -2;
            return;
        }
    }

    return sizeOfRows;
}

char** getRows(int file, int* errorDetected){
    int numOfRows = numRows(file, errorDetected);

    char** rows = (char**)malloc((numOfRows + 1) * sizeof(char*));
    if(rows == NULL){
        errorDetected = -3;
        return;
    }
    rows[numOfRows] = NULL;

    int* sizeOfRows = sizeRows(file, errorDetected);

    for(int i = 0; i < numOfRows; i++){
        rows[i] = (char*)malloc((sizeOfRows[i] + 1) * sizeof(char));
        if(rows[i] == NULL){
            errorDetected = -3;
            return;
        }
        rows[i][sizeOfRows[i]] = '\0';
    }

    int rowIndex = 0;
    int charIndex = 0;

    if(lseek(file, 0, SEEK_SET) == -1){
        errorDetected = -1;
        return;
    }

    char symbol;
    int readStatus = read(file, &symbol, 1);
    if(readStatus == -1){
        errorDetected = -2;
        return;
    }

    while(readStatus == 1){
        if(symbol == '\n'){
            rowIndex++;
            charIndex = 0;
        } else {
            rows[rowIndex][charIndex] = symbol;
            charIndex++;
        }
        readStatus = read(file, &symbol, 1);
        if(readStatus == -1){
            errorDetected = -2;
            return;
        }
    }

    return rows;
}

int numWords(char* row, int* errorDetected){
    int numOfWords = 0;

    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            numOfWords++;
        }
    }

    numOfWords++;

    return numOfWords;
}

int* sizeWords(char* row, int* errorDetected){
    int numOfWords = numWords(row, errorDetected);

    int* sizeOfWords = (int*)malloc(numOfWords * sizeof(int));
    if(sizeOfWords == NULL){
        errorDetected = -3;
        return;
    }

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

char** getWords(char* row, int* errorDetected){
    int numOfWords = numWords(row, errorDetected);

    char** words = (char**)malloc((numOfWords + 1) * sizeof(char*));
    if(words == NULL){
        errorDetected = -3;
        return;
    }
    words[numOfWords] = NULL;

    int* sizeOfWords = sizeWords(row, errorDetected);

    for(int i = 0; i < numOfWords; i++){
        words[i] = (char*)malloc((sizeOfWords[i] + 1) * sizeof(char));
        if(words[i] == NULL){
            errorDetected = -3;
            return;
        }
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

void errorHandler(int errorDetected){
    if(errorDetected == -1){
        err(1, "Error seeking in file");
    }
    else if(errorDetected == -2){
        err(1, "Error reading from file");
    }
    else if(errorDetected == -3){
        err(1, "Error allocating memory");
    }
    else if(errorDetected == -4){
        err(1, "Error forking");
    }
    else if(errorDetected == -5){
        err(1, "Error executing command");
    }
}

void executeCommands(char* filename, int* errorDetected){
    int fd = open(filename, O_RDONLY);
    int errorDetected = 0;
    if(fd == -1){
        err(1, "Error opening file");
    }

    int numOfRows = numRows(fd, errorDetected);
    if(errorDetected != 0){
        errorHandler(errorDetected);
    }
    char** rows = getRows(fd, errorDetected);
    if(errorDetected != 0){
        for(int i = 0; i < numOfRows; i++){
            free(rows[i]);
        }
        free(rows);

        errorHandler(errorDetected);
    }

    for(int i = 0; i < numOfRows; i++){
        char** words = getWords(rows[i], errorDetected);

        if(errorDetected != 0){
            for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                free(words[j]);
            }
            free(words);

            for(int j = 0; j < i; j++){
                free(rows[j]);
            }
            free(rows);

            errorHandler(errorDetected);
        }
        
        int childPid = fork();

        if(childPid == -1){
            errorDetected = -4;

            for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                free(words[j]);
            }
            free(words);

            for(int j = 0; j < i; j++){
                free(rows[j]);
            }
            free(rows);

            errorHandler(errorDetected);
        }
        else if(childPid == 0){
            if(execvp(words[0], words) == -1){
                errorDetected = -5;

                for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                    free(words[j]);
                }
                free(words);

                for(int j = 0; j < i; j++){
                    free(rows[j]);
                }
                free(rows);

                errorHandler(errorDetected);
            }
        }
        else{
            int status;
            int child = wait(&status);

            if(WIFEXITED(status)){
                printf("Child %d exited with status %d\n", child, WEXITSTATUS(status));
            }
        }

        if(i == numOfRows - 1){
            for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                free(words[j]);
            }
            free(words);
        }
    }

    for(int i = 0; i < numOfRows; i++){
        free(rows[i]);
    }
    free(rows);

    close(fd);
}