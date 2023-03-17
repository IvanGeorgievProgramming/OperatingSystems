#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <err.h>
#include <string.h>

// Error handler
void errorHandler(int err) {
    switch(err){
        case -1:
            printf("Error: Invalid number of arguments\n");
            exit(1);
        case -2:
            printf("Error: Could not allocate memory\n");
            exit(1);
        case -3:
            printf("Error: Could not open file\n");
            exit(1);
        case -4:
            printf("Error: Could not read file\n");
            exit(1);
        case -5:
            printf("Error: Could not write file\n");
            exit(1);
        case -6:
            printf("Error: Could not move file pointer\n");
            exit(1);
        case -7:
            printf("Error: Invalid number of points\n");
            exit(1);
        default:
            printf("Error: Unknown error\n");
            exit(1);
    }
}

void checkNumArgs(int argc, int err){
    // If the number of arguments is not 4, print an error message and exit
    if (argc != 4) {
        err = -1;
        return;
    }
}

char* combineTwoStrings(char* str1, char* str2, int* err){
    // Get the length of the first string
    int len1 = strlen(str1);
    // Get the length of the second string
    int len2 = strlen(str2);

    // Allocate memory for the combined string
    char* combined = (char*)malloc((len1 + len2 + 1) * sizeof(char));
    if(combined == NULL){
        err = -2;
        return NULL;
    }

    // Copy the first string into the combined string using strcpy
    strcpy(combined, str1);

    // Copy the second string into the combined string using strcpy
    strcpy(combined + len1, str2);

    // Return the combined string
    return combined;
}

void getAnswers(char* filename, char* answers, int* err){
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err = -3;
        return;
    }

    int res;
    char buffer;

    for(int i = 0; i < 25; i++){
        // Read the next character from the file
        res = read(fd, &buffer, 1);
        // Check if the read was successful
        if (res == -1) {
            err = -4;
            return;
        }
        
        // Store the character in the answers array
        answers[i] = buffer;

        // Move the file pointer to the next character
        res = lseek(fd, 1, SEEK_CUR);
        // Check if the move was successful
        if(res == -1){
            err = -6;
            return;
        }
    }

    // Close the file
    close(fd);
}

char* fromIntToString(int num, int* err){
    // Check how big is the number
    int digits = 0;
    int temp = num;
    while(temp != 0){
        temp /= 10;
        digits++;
    }

    // Allocate memory for the string
    char* str = (char*)malloc((digits + 1) * sizeof(char));
    if(str == NULL){
        err = -2;
        return NULL;
    }

    // Convert the integer to a string
    sprintf(str, "%d", num);

    // Return the string
    return str;
}

int compareStrings(char* str1, char* str2){
    int count = 0;
    for(int i = 0; i < 25; i++){
        if(str1[i] == str2[i]){
            count++;
        }
    }
    return count;
}

char* pointsToGrade(int points, int* err){
    if(points >= 22 && points <= 25){
        return "6";
    }
    else if(points >= 19 && points <= 21){
        return "5";
    }
    else if(points >= 16 && points <= 18){
        return "4";
    }
    else if(points >= 13 && points <= 15){
        return "3";
    }
    else if(points >=0 && points <= 12){
        return "2";
    }
    else{
        err = -7;
        return NULL;
    }
}

// Make a  function to write the results to a file
void writeResults(char* filename, char* grade, int* err){
    int fd = open(filename, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        err = -3;
        return;
    }

    int res;
    char buffer;

    // Write the grade to the file
    res = write(fd, grade, 1);
    // Check if the write was successful
    if (res == -1) {
        close(fd);
        err = -5;
        return;
    }

    // Close the file
    close(fd);
}

int main(int argc, char** argv){
    int err = 0;
    // Check the number of arguments
    checkNumArgs(argc, &err);
    if(err != 0){
        errorHandler(err);
    }

    // Make a dynamic array for the file name of the correct answers
    char* FileCorrectAnswers = combineTwoStrings(argv[2], "correct_answers", &err);
    if(FileCorrectAnswers == NULL){
        errorHandler(err);
    }
    // Make a dynamic array to store the correct answers
    char* correctAnswers = (char*)malloc(25 * sizeof(char));
    if(correctAnswers == NULL){
        free(FileCorrectAnswers);
        errorHandler(-2);
    }

    // Get the correct answers from the file
    getAnswers(FileCorrectAnswers, correctAnswers, &err);
    if(err != 0){
        free(FileCorrectAnswers);
        free(correctAnswers);
        errorHandler(err);
    }

    int points = 0;
    for(int i = 0; i < 25; i++){
        // Make a dynamic array for the file name of the student answers
        char* FileStudentAnswers = combineTwoStrings(argv[1], fromIntToString((i + 1), &err), &err);
        if(FileStudentAnswers == NULL){
            free(FileCorrectAnswers);
            free(correctAnswers);
            errorHandler(err);
        }
        // Make a dynamic array for the file name of the student results
        char* FileStudentResults = combineTwoStrings(argv[3], fromIntToString((i + 1), &err), &err);
        if(FileStudentResults == NULL){
            free(FileCorrectAnswers);
            free(correctAnswers);
            free(FileStudentAnswers);
            errorHandler(err);
        }
        // Make a dynamic array to store the student answers
        char* studentAnswers = (char*)malloc(25 * sizeof(char));
        if(studentAnswers == NULL){
            free(FileCorrectAnswers);
            free(correctAnswers);
            free(FileStudentAnswers);
            free(FileStudentResults);
            errorHandler(-2);
        }

        getAnswers(FileStudentAnswers, studentAnswers, &err);
        if(err != 0){
            free(FileCorrectAnswers);
            free(correctAnswers);
            free(FileStudentAnswers);
            free(FileStudentResults);
            free(studentAnswers);
            errorHandler(err);
        }

        points = compareStrings(correctAnswers, studentAnswers);
        if(err != 0){
            free(FileCorrectAnswers);
            free(correctAnswers);
            free(FileStudentAnswers);
            free(FileStudentResults);
            free(studentAnswers);
            errorHandler(err);
        }

        char* grade = pointsToGrade(points, &err);
        if(err != 0){
            free(FileCorrectAnswers);
            free(correctAnswers);
            free(FileStudentAnswers);
            free(FileStudentResults);
            free(studentAnswers);
            errorHandler(err);
        }

        writeResults(FileStudentResults, grade, &err);
        if(err != 0){
            free(FileCorrectAnswers);
            free(correctAnswers);
            free(FileStudentAnswers);
            free(FileStudentResults);
            free(studentAnswers);
            errorHandler(err);
        }

        free(FileCorrectAnswers);
        free(correctAnswers);
        free(FileStudentAnswers);
        free(FileStudentResults);
        free(studentAnswers);
    }
    

    return 0;
}