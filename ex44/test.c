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
// random numbers:
#include <time.h>
#include <stdint.h>

int main(int argc, char * argv[]) {
	int fdf1 = open("f1", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	int fdf2 = open("f2", O_WRONLY | O_TRUNC | O_CREAT, 0644);

	// Seed randomness with time insuring different seeds and thus different data.
	srand(time(NULL));

	// Init f2 data:
	uint32_t elements_in_f2_count = 100;
	uint32_t current_element;
	for (unsigned int iterator = 0; iterator < elements_in_f2_count; iterator++) {
		current_element = iterator;
		write(fdf2, &current_element, 4);
	}

	// Generate intervals:
	uint8_t intervals_count = 3;
	uint32_t interval_start, interval_length;
	for (unsigned int iterator = 0; iterator < intervals_count; iterator++) {
		interval_start = rand() % elements_in_f2_count;
		interval_length = (rand() % (elements_in_f2_count - interval_start)) % 10;
		write(fdf1, &interval_start, 4);
		write(fdf1, &interval_length, 4);
	}

	exit(0);
}