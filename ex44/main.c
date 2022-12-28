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

int main(int argc, char * argv[]) {
	int fdf1 = open("f1", O_RDONLY);
	if ( -1 == fdf1 ) {
		err(1, "Unable to open f1");
	}

	int fdf2 = open("f2", O_RDONLY);
	if ( -1 == fdf2 ) {
		close(fdf1);
		err(2, "Unable to open f2");
	}

	int fdf3 = open("f3", O_WRONLY | O_TRUNC);
	if ( -1 == fdf3 ) {
		close(fdf1);
		close(fdf2);
		err(3, "Unable to open f3");
	}

	unsigned char interval[8];
	int interval_reader_status;
	uint32_t interval_start, interval_length;
	while ( 8 == (interval_reader_status = read(fdf1, interval, 8)) ) {
		interval_start = *((uint32_t *)interval);
		interval_length = *((uint32_t *)(interval + 4));

		// Possition f2 cursor at the start of the interval:
		if ( -1 == lseek(fdf2, 4 * interval_start, SEEK_SET) ) {
			close(fdf1);
			close(fdf2);
			close(fdf3);
			err(6, "Could not lseek in f2");
		}
		// Alloc memory for the interval:
		uint32_t * buffer = malloc(interval_length * 4);
		if ( NULL == buffer ) {
			close(fdf1);
			close(fdf2);
			close(fdf3);
			err(7, "Could not alloc memory");
		}

		// Read the entire interval at once to reduce the number of read() calls.
		if ( interval_length * 4 != read(fdf2, buffer, interval_length * 4) ) {
			close(fdf1);
			close(fdf2);
			close(fdf3);
			free(buffer);
			err(8, "Could not read an interval from f2");
		}

		// Once the interval is read in the RAM, write in to f3. Again, minimize the number
		// of write() calls.
		if ( interval_length * 4 != write(fdf3, buffer, interval_length * 4) ) {
			close(fdf1);
			close(fdf2);
			close(fdf3);
			free(buffer);
			err(9, "Could not write an interval to f3");
		}

		free(buffer);
	}
	if ( -1 == interval_reader_status ) {
		close(fdf1);
		close(fdf2);
		close(fdf3);
		err(4, "Unable to read data from f1");
	}
	if ( 0 != interval_reader_status ) {
		close(fdf1);
		close(fdf2);
		close(fdf3);
		errx(5, "Incompleate interval definition in f1, erroring out!");
	}

	close(fdf1);
	close(fdf2);
	close(fdf3);

	exit(0);
}

/*
Linux - Compile and run:

gcc ex44/main.c
./a.out
*/