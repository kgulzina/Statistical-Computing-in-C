/**
 * @file main.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION
 *
 * Usage:
 *	./qgamma -p <double in [0, 1]> -a <double positive> -b <double positive>
 *
 * Compile as:
 *	gcc *.c -Wall -pedantic -o qgamma -lm;
 *
 * One-step example:
 *	rm qgamma; gcc *.c -Wall -pedantic -o qgamma -lm; ./qgamma -p 0.57 \
 *	-a 0.5 -b 3
 *
 * Troubleshooting:
 *	If program prints a FATAL error, kindly follow the instructions.
 *	If still in trouble, please try rebooting your system.
 *	If error persists, shut down computer, wait 10 seconds, turn on again.
 *
 * Warranty:
 *	Limited warranty. If you try very large or small values, you break it.
 */

/* gcc *.c -Wall -pedantic; ./a.out /home/luis/Downloads/SRR2990088_1_noN.fastq -n 50; rm ./a.out */

#include <stdio.h>
#include <stdlib.h> /* exit codes */
#include <string.h> /* strcpy, strcat */
#include "main.h"
#include "bisection.h" /* bisection */
#include "hashing.h"
#include "interface.h"
#include "storage.h" /* ascii2bin */
#include "mle.h"

int main(int argc, char **argv)
{
#ifdef DEBUG
	printf("DEBUG: Debug mode was set by the gods at compile time.\n");
#endif

	/* Read user-specified arguments */
	char pathIn[MAX_FILENAME_LENGTH];
	int maxReadings = -1; /* Read every line by default */

	read_argv(argc, argv, &maxReadings, pathIn);

#ifdef DEBUG
	printf("DEBUG: I will read the first n = %i rows from %s. "
			"Wish me luck!\n", maxReadings, pathIn);
#endif

	/* Do something useful already */
	char pathOut[MAX_FILENAME_LENGTH];

	strcpy(pathOut, pathIn);
	strcat(pathOut, "bin");

	ascii2bin(pathIn, pathOut, maxReadings);

	int nReadings;
	reading *readings = 0;

	load_bin(pathOut, &readings, &nReadings);

	int nUnique = 0;
	int *idUnique = NULL;
	int *countsUnique = NULL;

	crazyhashes(readings, nReadings, &idUnique, &countsUnique, &nUnique);

#ifdef DEBUG
	printf("DEBUG: Found a total of %i unique out of %i total sequences\n",
			nUnique, nReadings);

	for (int i = 0; i < nUnique; i++)
		printf("\t[% 3i] Sequence at pos % 3i was read % 3i times.\n",
				i, idUnique[i], countsUnique[i]);
#endif

	printf("Passing %i counts.\n", nUnique);

	printf("WECOLME!\n");
	printf("These are the MLE for Alpha:\n");

	/* 2.b */
	int* params[] = {countsUnique, &nUnique};
	double out = bisection(score_function, (void *)params, 1E-6, 10, 0,
			1E-4);

	printf("  Bisectionally(?): alpha =% .15e\n", out);

	/*	printf("BIC = % 4.4f\n", BIC(countsUnique, nUnique, 1.5434, 2)); */

	/* 2.c */

#define XMIN_GRID 2
	for (int xmin = 1; xmin < XMIN_GRID; xmin++) {
		int* params2[] = {countsUnique, &nUnique, &xmin};
		double out2 = bisection(bic_score_function, (void *)params2,
				1E-6, 200000,
				0, 1E-4);

		printf("  Bisectionally(xmin = %i): alpha =% .15e\n", xmin,
				out2);
	}

	/* 2.d */
	reading *readingsUnique = malloc(nUnique * sizeof(reading));
	for (int i = 0; i < nUnique; i++)
		readingsUnique[i] = readings[idUnique[i]];

	double alpha = 0;
	alpha = mle_alpha(readingsUnique, nUnique);
	printf("  Analytically    : alpha =% .15e\n", alpha);

	free(readings);

	return EXIT_SUCCESS; /* @suppress("Return with parenthesis") */
}
