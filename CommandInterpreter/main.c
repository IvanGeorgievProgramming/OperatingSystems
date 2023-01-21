// * Importing libraries

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

// * Function declarations

int numRows(int file, int* errorDetected);
int* sizeRows(int file, int* errorDetected);
char** getRows(int file, int* errorDetected);
int numWords(char* row, int* errorDetected);
int* sizeWords(char* row, int* errorDetected);
char** getWords(char* row, int* errorDetected);
void errorHandler(int errorDetected);
void executeCommands(char* filename);

// * Main function

int main(int argc, char *argv[]){
    // Check if the number of arguments is correct
    if(argc != 2){
        // If not, print an error message and exit
        errx(1, "Invalid number of arguments");
    }
    
    // If the number of arguments is correct, execute the commands
    executeCommands(argv[1]);
}

// * Function definitions

// Function that gets the number of rows in a file
int numRows(int file, int* errorDetected){
    // Seek to the beginning of the file
    if(lseek(file, 0, SEEK_SET) == -1){
        errorDetected = -1;
        return;
    }

    int lines = 0;
    char symbol;

    // Read the first symbol
    int readStatus = read(file, &symbol, 1);
    if(readStatus == -1){
        errorDetected = -2;
        return;
    }

    // While there are symbols to read
    while(readStatus == 1){
        // If the symbol is a new line, increment the number of rows
        if(symbol == '\n'){
            lines++;
        }
        // Read the next symbol
        readStatus = read(file, &symbol, 1);
        if(readStatus == -1){
            errorDetected = -2;
            return;
        }
    }

    // Increment the number of rows to account for the last row
    lines++;

    // Return the number of rows
    return lines;
}

// Function that gets the size of each row in a file
int* sizeRows(int file, int* errorDetected){
    // Get the number of rows in the file
    int numOfRows = numRows(file, errorDetected);

    // Allocate memory for the array that will hold the size of each row
    int* sizeOfRows = (int*)malloc(numOfRows * sizeof(int));
    if(sizeOfRows == NULL){
        errorDetected = -3;
        return;
    }

    int rowIndex = 0;
    int charIndex = 0;

    // Seek to the beginning of the file
    if(lseek(file, 0, SEEK_SET) == -1){
        errorDetected = -1;
        return;
    }

    char symbol;

    // Read the first symbol
    int readStatus = read(file, &symbol, 1);
    if(readStatus == -1){
        errorDetected = -2;
        return;
    }

    // While there are symbols to read
    while(readStatus == 1){
        // If the symbol is a new line, increment the row index and reset the character index
        if(symbol == '\n'){
            sizeOfRows[rowIndex] = charIndex;
            rowIndex++;
            charIndex = 0;
        } 
        // If the symbol is not a new line, increment the character index
        else {
            charIndex++;
        }
        // Read the next symbol
        readStatus = read(file, &symbol, 1);
        if(readStatus == -1){
            errorDetected = -2;
            return;
        }
    }

    return sizeOfRows;
}

// Function that gets the rows of a file
char** getRows(int file, int* errorDetected){
    // Get the number of rows in the file
    int numOfRows = numRows(file, errorDetected);

    // Allocate memory for the array that will hold the rows
    char** rows = (char**)malloc((numOfRows + 1) * sizeof(char*));
    if(rows == NULL){
        errorDetected = -3;
        return;
    }

    // Set the last element of the array to NULL
    rows[numOfRows] = NULL;

    // Get the size of each row
    int* sizeOfRows = sizeRows(file, errorDetected);

    // Allocate memory for each row
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

    // Seek to the beginning of the file
    if(lseek(file, 0, SEEK_SET) == -1){
        errorDetected = -1;
        return;
    }

    char symbol;
    // Read the first symbol
    int readStatus = read(file, &symbol, 1);
    if(readStatus == -1){
        errorDetected = -2;
        return;
    }

    // While there are symbols to read
    while(readStatus == 1){
        // If the symbol is a new line, increment the row index and reset the character index
        if(symbol == '\n'){
            rowIndex++;
            charIndex = 0;
        }
        // If the symbol is not a new line, add it to the current row and increment the character index 
        else {
            rows[rowIndex][charIndex] = symbol;
            charIndex++;
        }
        // Read the next symbol
        readStatus = read(file, &symbol, 1);
        if(readStatus == -1){
            errorDetected = -2;
            return;
        }
    }

    return rows;
}

// Function that gets the number of words in a row
int numWords(char* row, int* errorDetected){
    int numOfWords = 0;

    // Count the number of spaces in the row
    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            numOfWords++;
        }
    }

    // Increment the number of words to account for the last word
    numOfWords++;

    // Return the number of words
    return numOfWords;
}

// Function that gets the size of each word in a row
int* sizeWords(char* row, int* errorDetected){
    // Get the number of words in the row
    int numOfWords = numWords(row, errorDetected);

    // Allocate memory for the array that will hold the size of each word
    int* sizeOfWords = (int*)malloc(numOfWords * sizeof(int));
    if(sizeOfWords == NULL){
        errorDetected = -3;
        return;
    }

    int wordSize = 0;
    int wordIndex = 0;

    // Count the number of characters in each word
    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            sizeOfWords[wordIndex] = wordSize;
            wordIndex++;
            wordSize = 0;
        } else {
            wordSize++;
        }
    }

    // Add the size of the last word to the array
    sizeOfWords[wordIndex] = wordSize;

    // Return the array
    return sizeOfWords;
}

// Function that gets the words of a row
char** getWords(char* row, int* errorDetected){
    // Get the number of words in the row
    int numOfWords = numWords(row, errorDetected);

    // Allocate memory for the array that will hold the words
    char** words = (char**)malloc((numOfWords + 1) * sizeof(char*));
    if(words == NULL){
        errorDetected = -3;
        return;
    }
    // Set the last element of the array to NULL
    words[numOfWords] = NULL;

    // Get the size of each word
    int* sizeOfWords = sizeWords(row, errorDetected);

    // Allocate memory for each word
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

    // Get each word
    for(int i = 0; row[i] != '\0'; i++){
        if(row[i] == ' '){
            wordIndex++;
            charIndex = 0;
        } else {
            words[wordIndex][charIndex] = row[i];
            charIndex++;
        }
    }

    // Return the array
    return words;
}

// Error handler
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

// Function that executes the commands from a file
void executeCommands(char* filename, int* errorDetected){
    // Open the file
    int errorDetected = 0;
    int fd = open(filename, O_RDONLY);
    // If the file could not be opened, print an error message and exit
    if(fd == -1){
        err(1, "Error opening file");
    }

    // Get the number of rows in the file
    int numOfRows = numRows(fd, errorDetected);
    if(errorDetected != 0){
        errorHandler(errorDetected);
    }
    // Get the rows of the file
    char** rows = getRows(fd, errorDetected);
    if(errorDetected != 0){
        // Free the memory allocated for the rows
        for(int i = 0; i < numOfRows; i++){
            free(rows[i]);
        }
        free(rows);

        errorHandler(errorDetected);
    }

    for(int i = 0; i < numOfRows; i++){
        // Get the words of the current row
        char** words = getWords(rows[i], errorDetected);

        if(errorDetected != 0){
            // Free the memory allocated for the words
            for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                free(words[j]);
            }
            free(words);

            // Free the memory allocated for the rows
            for(int j = 0; j < i; j++){
                free(rows[j]);
            }
            free(rows);

            errorHandler(errorDetected);
        }
        
        // Fork the current process
        int childPid = fork();

        // If the fork failed
        if(childPid == -1){
            errorDetected = -4;

            // Free the memory allocated for the words
            for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                free(words[j]);
            }
            free(words);

            // Free the memory allocated for the rows
            for(int j = 0; j < i; j++){
                free(rows[j]);
            }
            free(rows);

            errorHandler(errorDetected);
        }
        // If the current process is the child
        else if(childPid == 0){
            // Execute the command and check for errors
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
        // If the current process is the parent
        else{
            // Wait for the child to finish
            int status;
            int child = wait(&status);

            if(WIFEXITED(status)){
                printf("Child %d exited with status %d\n", child, WEXITSTATUS(status));
            }
        }

        // If the current row is the last one
        if(i == numOfRows - 1){
            // Free the memory allocated for the words
            for(int j = 0; j < numWords(rows[i], errorDetected); j++){
                free(words[j]);
            }
            free(words);
        }
    }

    // Free the memory allocated for the rows
    for(int i = 0; i < numOfRows; i++){
        free(rows[i]);
    }
    free(rows);

    close(fd);
}