/**
 * @file storage.h
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Hashing functions.
 */

#ifndef HASHING_H_
#define HASHING_H_

#include "storage.h" /* reading */
#include "uthash.h" /* UT_hash_handle */

typedef struct _dna_entry dna_entry;

struct _dna_entry {
	char *sequence;
	int count;
	UT_hash_handle hh;
};

void crazyhashes(reading *readings, int nReadings, int **idUnique,
		int **countsUnique, int *nUnique);

#endif /* HASHING_H_ */
