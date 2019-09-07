/**
 * @file storage.h
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Storage (read and write) functions.
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "main.h"

typedef struct {
	int n; /* length of the sequences*/
	char* sequence; /* an array of size n with the nucleotide sequence */
	char* scores; /* an array of size n with the quality scores */
} reading;

void ascii2bin(char *filenameIn, char *filenameOut, int n);
void load_bin(char *pathIn, reading **to, int *n);

#endif /* STORAGE_H_ */
