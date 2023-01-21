//exit, read, write, close
#include <unistd.h>
// open:
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
// printf:
#include<stdio.h>
// exit
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<err.h>

int numRows(int file);
int numWords(char* row);
int* sizeOfWords(char* row);
char** getWords(char* row);
void executeCommands();

int main(int argc, char *argv[]){
    // Make a string
    char* str = "Hello World , my name is Ivan";

    // Get the number of words in the string
    int numOfWords = numWords(str);
    printf("The words are: %d\n", numOfWords);

    // Get the size of each word in the string
    int* sizeOfWordsArr = sizeOfWords(str);
    printf("The size of each word is: ");
    for (int i = 0; i < numOfWords; i++){
        printf("%d ", sizeOfWordsArr[i]);
    }
    printf("\n");

    // Get all the words in the string
    char** words = getWords(str);
    printf("The words are: ");
    for (int i = 0; i < numOfWords; i++){
        printf("%s ", words[i]);
    }
    printf("\n");

    // Get the number of rows in a file
    executeCommands();
}

// Make a function that counts the number of rows in a file.
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

// Make a function that counts the number of words in a string.
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

// Make a function that counts the size of each word in a string.
int* sizeOfWords(char* row){
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

// Make a function that returns all the words in a string.
char** getWords(char* row){
    int numOfWords = numWords(row);

    char** words = (char**)malloc((numOfWords + 1) * sizeof(char*));
    words[numOfWords] = NULL;

    int* sizeOfWordsArr = sizeOfWords(row);

    for(int i = 0; i < numOfWords; i++){
        words[i] = (char*)malloc((sizeOfWordsArr[i] + 1) * sizeof(char));
        words[i][sizeOfWordsArr[i]] = '\0';
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

// Make a function to execute the commands.
void executeCommands(){
    int fd = open("CommandInterpreter/commands", O_RDONLY);
    if(fd == -1){
        err(1, "Error opening file");
    }

    int numOfRows = numRows(fd);
    printf("The number of rows is: %d\n", numOfRows);
}