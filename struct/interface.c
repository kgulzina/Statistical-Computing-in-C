/**
 * @file read.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Interface (read and print) functions. Use responsibly :)
 */

#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* strcpy */
#include <unistd.h> /* getopt */
#include "interface.h"

/**
 * Read the command line arguments -p, -a, -b.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param **argv Pointer to the first element of an array of pointers
 * to null-terminated multibyte strings that represent the arguments passed
 * to the program from the execution environment.
 * @param *action Address of the integer where the action code should
 * be written.
 * @param *filename Address of the character array where the filename should
 * be written.
 * @see https://en.cppreference.com/w/c/language/main_function for more
 * information about argc and **argv.
 */
void read_argv(int argc, char **argv, int *n, char *filename)
{
	int c;

	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
		case 'n':
			sscanf(optarg, "%i", n);
			break;
		case '?':
			fatal("unknown option");
		}
	}

	strcpy(filename, argv[optind]);
}

/**
 * Print a fatal error message and exit with failure.
 * @param message Error message.
 */
void fatal(char *message)
{
	fprintf(stderr, "FATAL ERROR: %s.\n", message);
	fprintf(stderr,
			"\nUsage:\n	./seq -n <int nonnegative> filename\n");

	exit(EXIT_FAILURE);
}

/**
 * Print a warning message.
 * @param message Warning message.
 */
void warning(char *message)
{
	fprintf(stderr, "WARNING: %s.\n", message);
}
