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

//! Should check every possible error condition
//! Every file descriptor should be closed
//! Every malloc should be freed

void checkNumArgs(int argc){
    // If the number of arguments is not 4, print an error message and exit
    if (argc != 4) {
        printf("Error: Invalid number of arguments\n");
        exit(1);
    }
}

char* combineTwoStrings(char* str1, char* str2){
    // Get the length of the first string
    int len1 = strlen(str1);
    // Get the length of the second string
    int len2 = strlen(str2);

    // Allocate memory for the combined string
    char* combined = (char*)malloc((len1 + len2 + 1) * sizeof(char));
    if(combined == NULL){
        printf("Error: Could not allocate memory");
        exit(1);
    }

    // Copy the first string into the combined string using strcpy
    strcpy(combined, str1);

    // Copy the second string into the combined string using strcpy
    strcpy(combined + len1, str2);

    // Return the combined string
    return combined;
}

void getAnswers(char* filename, char* answers){
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Error: Could not open file %s", filename);
        exit(1);
    }

    int res;
    char buffer;

    for(int i = 0; i < 25; i++){
        // Read the next character from the file
        res = read(fd, &buffer, 1);
        // Check if the read was successful
        if (res == -1) {
            printf("Error: Could not read from file %s", filename);
            close(fd);
            exit(1);
        }

        // Store the character in the answers array
        answers[i] = buffer;

        // Move the file pointer to the next character
        res = lseek(fd, 1, SEEK_CUR);
        // Check if the move was successful
        if(res == -1){
            printf("Error: Could not move file pointer");
            close(fd);
            exit(1);
        }
    }

    // Close the file
    close(fd);

}

char* fromIntToString(int num){
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
        printf("Error: Could not allocate memory");
        exit(1);
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

char* pointsToGrade(int points){
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
        printf("Error: Invalid number of points");
        exit(1);
    }
}

// Make a  function to write the results to a file
void writeResults(char* filename, char* grade){
    int fd = open(filename, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        printf("Error: Could not open file %s", filename);
        exit(1);
    }

    int res;
    char buffer;

    // Write the grade to the file
    res = write(fd, grade, 1);
    // Check if the write was successful
    if (res == -1) {
        printf("Error: Could not write to file %s", filename);
        close(fd);
        exit(1);
    }

    // Close the file
    close(fd);
}

int main (int argc, char** argv) {

    // Check the number of arguments
    checkNumArgs(argc);

    // Make a dynamic array for the file name of the correct answers
    char* FileCorrectAnswers = combineTwoStrings(argv[2], "correct_answers");
    // Make a dynamic array to store the correct answers
    char* correctAnswers = (char*)malloc(25 * sizeof(char));
    if(correctAnswers == NULL){
        printf("Error: Could not allocate memory");
        exit(1);
    }

    // Get the correct answers from the file
    getAnswers(FileCorrectAnswers, correctAnswers);

    int points;

    for(int i = 0; i < 25; i++){
        // Make a dynamic array for the file name of the student answers
        char* FileStudentAnswers = combineTwoStrings(argv[1], fromIntToString(i + 1));
        // Make a dynamic array for the file name of the student results
        char* FileStudentResults = combineTwoStrings(argv[3], fromIntToString(i + 1));
        // Make a dynamic array to store the student answers
        char* studentAnswers = (char*)malloc(25 * sizeof(char));
        if(studentAnswers == NULL){
            printf("Error: Could not allocate memory");

            free(FileCorrectAnswers);
            free(FileStudentAnswers);
            free(FileStudentResults);
            exit(1);
        }
        getAnswers(FileStudentAnswers, studentAnswers);
        // Make a dynamic array to store the student results
        points = compareStrings(correctAnswers, studentAnswers);
        // Make a dynamic array to store the student grade
        char* grade = pointsToGrade(points);
        // Write the results to a file
        writeResults(FileStudentResults, grade);

        // Free the memory
        free(FileCorrectAnswers);
        free(FileStudentAnswers);
        free(FileStudentResults);
        
        // Господине не знам защо ми дава грешка при тези free-ове:
        // free(correctAnswers);
        // free(studentAnswers);
    }

    return 0;
}