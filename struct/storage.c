/**
 * @file storage.
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Storage (read and write) functions.
 */

#include <stdio.h> /* FILE */
#include <string.h> /* strlen */
#include <stdlib.h> /* exit */
#include "main.h" /* BUFFER_SIZE */
#include "interface.h" /* fatal */
#include "storage.h"

/**
 * Reads the first n lines from pathIn (ASCII) and writes a reduced version
 * to pathOut (binary).
 * @param pathIn Path to the ASCII file to read from.
 * @param pathOut Path to the binary file to write to.
 * @param n Number of lines to process. Set to -1 to read all the lines.
 * @return The number of lines.
 * @note If pathOut file exists, it will be completely overwritten.
 */
void ascii2bin(char *pathIn, char *pathOut, int nReadings)
{
	FILE *fIn, *fOut;

	fIn = fopen(pathIn, "r");
	if (!fIn)
		fatal("could not open the ASCII input filename");

	fOut = fopen(pathOut, "wb");
	if (!fOut)
		fatal("could not create the binary output filename");

	char b1[BUFFER_SIZE];
	char b2[BUFFER_SIZE];
	char b3[BUFFER_SIZE];
	char b4[BUFFER_SIZE];
	int len; /* length of the sequence */

	while (nReadings-- != 0) {
		if (!fgets(b1, BUFFER_SIZE, fIn)
				| !fgets(b2, BUFFER_SIZE, fIn)
				| !fgets(b3, BUFFER_SIZE, fIn)
				| !fgets(b4, BUFFER_SIZE, fIn))
			break;

		len = strlen(b2);

		if (len == BUFFER_SIZE)
			fatal("could not load the entire nucleotide reading"
					"-- increase BUFFER_SIZE");

		/* NOTE: We keep the trailing LF in the scores strings to
		 * later count the number of readings in the binary file
		 * (one per LF). The modifier len - 1 removes the trailing LF
		 * from the sequence strings though.
		 */
		fprintf(fOut, "%d%.*s%s", len, len - 1, b2, b4);

#ifdef DEBUG
		printf("DEBUG: Read from ASCII file and written to Binary "
				"file\n\tReading  %d\n\tLength   "
				"%d\n\tSequence %s\tScores   %s\n",
				nReadings, len, b2, b4);
#endif
	}

	fclose(fIn);
	fclose(fOut);

#ifdef DEBUG
	printf("DEBUG: Conversion ASCII to binary is done.\n");
#endif
}

void load_bin(char *pathIn, reading **to, int *n)
{
	char c = 'c'; /* trash */
	int flag = 1;

	FILE *fIn;

	fIn = fopen(pathIn, "rb");
	if (!fIn)
		fatal("could not open the binary file");

	/* Determine the number of sequences */
	*n = 0; /* number of sequences */
	do {
		c = fgetc(fIn);

		if (c == '\n')
			(*n)++;
		else if (c == EOF)
			flag = 0;

	} while (flag);

#ifdef DEBUG
	printf("DEBUG: I've found n = %i nucleotide readings in "
			"the bin file %s.\n", *n, pathIn);
#endif

	/* Allocate memory */
	*to = malloc(*n * sizeof(reading));

	if ((*to) == NULL)
		fatal("cannot allocate array struct for readings "
				"-- out of memory");

	for (int i = 0; i < *n; i++) {
		(*to)[i].sequence = malloc(
		BUFFER_SIZE * member_size(reading, sequence));
		(*to)[i].scores = malloc(
		BUFFER_SIZE * member_size(reading, scores));

		if ((*to)[i].sequence == NULL || (*to)[i].scores == NULL)
			fatal("cannot allocate array struct for readings "
					"-- out of memory");
	}

	/* Read the sequence into the struct array */
	fseek(fIn, 0, SEEK_SET);

	/* NOTE: After this point, none of the sequence and scores strings
	 * have a trailing LR.
	 */
	for (int i = 0; i < *n; i++) {
		/* 1. Read length */
		if (fscanf(fIn, "%i", &((*to)[i].n)) != 1)
			fatal("can't read the sequence length from bin file");

		/* 2. Read nucleotide sequence */
		if (fgets((*to)[i].sequence, (*to)[i].n, fIn) == NULL)
			fatal("can't read the sequence from bin file");

		/* 3. Read quality scores*/
		if (fgets((*to)[i].scores, (*to)[i].n, fIn) == NULL)
			fatal("can't read the sequence from bin file");

#ifdef DEBUG
		printf("DEBUG: Read from Binary file\n\tLine     %d"
				"\n\tLength   %d\n\tSequence %s\n\t"
				"Scores   %s\n", i, (*to)[i].n,
				(*to)[i].sequence, (*to)[i].scores);
#endif
	}

	fclose(fIn);

#ifdef DEBUG
	int pos = *n / 2;

	printf("DEBUG: Example of the n = %i-th element of the in-memory "
			"array struct:\n\t"
			"Position %i\n\tLength   %i\n\tSequence %s\n\t"
			"Scores   %s\n", pos, pos, (*to)[pos].n,
			(*to)[pos].sequence, (*to)[pos].scores);

	printf("DEBUG: Loading the binary file to memory is done.\n");
#endif
}
