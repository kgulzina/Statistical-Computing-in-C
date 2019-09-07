/**
 * @file read.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Interface (read and print) functions. Use responsibly :)
 */

#include "interface.h"

#include <stdio.h>
#include <stdlib.h> /* exit */
#include <unistd.h> /* getopt */

/**
 * Read the command line arguments -n, -k, -m.
 * @param argc  Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param **argv Pointer to the first element of an array of pointers
 * to null-terminated multibyte strings that represent the arguments passed
 * to the program from the execution environment.
 * @param *n Address of the int variable where n should be written to.
 * @param *k Address of the int variable where k should be written to.
 * @param *m Address of the char variable where m should be written to.
 * @see https://en.cppreference.com/w/c/language/main_function for more
 * information about argc and **argv.
 */
void read_argv(int argc, char **argv, int *n, int *k, char *m)
{
	int c;

	while ((c = getopt(argc, argv, "n:k:m:")) != -1) {
		switch (c) {
		case 'k':
			sscanf(optarg, "%i", k);
			break;
		case 'n':
			sscanf(optarg, "%i", n);
			break;
		case 'm':
			sscanf(optarg, "%c", m);
			break;
		case '?':
			fatal("unknown option");
		}
	}

	if (*k < 0 || *n < 0)
		fatal("-k and -n are mandatory & nonnegative");

	if (*k > *n)
		fatal("k must be less than or equal to n");
}

/**
 * Read the dataset from stdin.
 * @param n Number of rows to read.
 * @param p Address of the myfloat array where the probabilities should
 * be written to.
 * @param len Address of the variable where k should be written to.
 * @warning THIS FUNCTION DOES NOT CHECK THE SIZE OF `p`. It is up to the
 * programmer to give an address to a myfloat array with enough preallocated
 * space to hold the first `n` values read from stdin.
 */
void read_data(int n, int maxLen, myfloat *p, int *len)
{
	char buffer[BUFFER_SIZE];
	int ret = 0;

	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		if (*len < n) {
			ret = sscanf(buffer, "%*u %"FLOAT_FMT_IN, &p[*len]);
			if (ret == 1)
				(*len)++;
		} else {
			sscanf(buffer, "%*u %*e");
			/* Left unchecked for optimization reasons */
		}
	}
}

/**
 * Print the first `n` rows to the probability vector.
 * @param n Number of rows to print.
 * @param p Address of the myfloat array where the probabilities are stored.
 * @warning THIS FUNCTION DOES NOT CHECK THE SIZE OF `p`. It is up to the
 * programmer to give an address to a myfloat array with enough preallocated
 * space to hold the first `n` values to be printed.
 */
void print_data(int n, myfloat *p)
{
	printf("First %i values of the probability vector.\n", n);

	printf("  n \t p_n\n");

	int i = 0;

	for (i = 0; i < n; i++)
		printf("  %i \t %" FLOAT_FMT_OUT "\n", i, p[i]);

	printf("  Total read: %i rows of data.\n\n", n);
}

/**
 * Print a fatal error message and exit with failure.
 * @param message Error message.
 */
void fatal(char *message)
{
	fprintf(stderr, "FATAL ERROR: %s.\n", message);
	fprintf(stderr,
			"./poisbin -n <int> -k <int> -m [nrt] < cat p.txt\n");

	exit(EXIT_FAILURE);
}
