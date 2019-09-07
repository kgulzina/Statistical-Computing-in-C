/**
 * @file storage.
 * @authors SSGL
 * @version 0.1
 * @details
 * @note Credits to Xiyu Peng, Karin Dorman
 * (this is an adaptation of their seq_hash file).
 *
 * Hashing functions.
 */

#include <stdio.h> /* printf */
#include "interface.h" /* fatal */
#include "storage.h" /* reading */
#include "hashing.h" /* dna_entry */

/**
 * Feed it some readings and it will count the total number of unique
 * sequences, count the number each sequence has been read, and identify the
 * position of the first element with each sequence.
 *
 * @param readings An array of readings.
 * @param nReadings The number of elements in the array.
 * @param idUnique A pointer to an array of integers where the position of the
 * first element of each unique sequence should be written.
 * @param countsUnique A pointer to an array of integers where the count of
 * each unique sequence should be written.
 * @param nUnique A pointer to an integer where the number of unique sequences
 * should be written.
 */
void crazyhashes(reading *readings, int nReadings, int **idUnique,
		int **countsUnique, int *nUnique)
{
	int seqSize = member_size(reading, sequence);
	dna_entry *hash = NULL;
	dna_entry *entry = NULL;

	/* In a single pass:
	 * (1) count the total number of unique sequences,
	 * (2) count the number each sequence has been read, and
	 * (3) identify the position of the first element with each sequence.
	 */
	*nUnique = 0;
	*idUnique = (int *)malloc(nReadings * sizeof(int));

	for (int i = 0; i < nReadings; i++) {
		/* check if sequence is already in hash */
		HASH_FIND(hh, hash, readings[i].sequence,
				readings[i].n * sizeof(readings[i].sequence),
				entry);

		if (entry) {
			/* (2) increment individual count */
			entry->count = entry->count + 1;
		} else { /* Entry is new, i.e. not in hash table */
			/* add entry to hash */
			entry = malloc(sizeof(*entry));
			if (!entry)
				fatal("Failed to allocate entry!\n");

			entry->sequence = readings[i].sequence;
			entry->count = 1;

			HASH_ADD_KEYPTR(hh, hash, entry->sequence,
					readings[i].n * seqSize,
					entry);

			/* (3) flag the position of the first appearance */
			/* (1) increment total count */
			(*idUnique)[(*nUnique)++] = i;
		}
	}

	/* Shrink array of positions */
	int *idUnique2 = realloc(*idUnique, *nUnique * sizeof(int));

	if (idUnique2 != NULL)
		*idUnique = idUnique2;
	else
		warning("couldn't shrink array, unnecessarily wasted memory");

	/* Count unique */
	*countsUnique = (int *)malloc(*nUnique * sizeof(int));

	for (int i = 0; i < *nUnique; i++) {
		entry = NULL;
		HASH_FIND(hh, hash, readings[(*idUnique)[i]].sequence,
				readings[(*idUnique)[i]].n * seqSize,
				entry);

		(*countsUnique)[i] = entry->count;
	}

	/* Clear hash table */
	dna_entry *tmp, *current;

	HASH_ITER(hh, hash, current, tmp)
	{
		if (current->sequence)
			free(current->sequence);
		HASH_DEL(hash, current);
		free(current);
	}

#ifdef DEBUG
	printf("DEBUG: All hashes in the hash table were deleted.");
#endif
}
