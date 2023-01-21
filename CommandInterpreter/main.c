#include <stdio.h>
#include <stdlib.h>

int numWords(char* row);
int* sizeOfWords(char* row);

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
