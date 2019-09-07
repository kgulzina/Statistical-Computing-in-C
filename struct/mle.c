/**
 * @file storage.
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Maximum likelihood estimation for the error count distribution.
 */

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <math.h> /* log */
#include "storage.h" /* reading */
#define MATHLIB_STANDALONE 1
#include <Rmath.h>
#include "bisection.h"

/**
 * Compute the MLE of the alpha parameter analytically.
 * @param readings An array of readings.
 * @param nReadings The number of elements in the array.
 * @return The MLE of the alpha parameter.
 */
double mle_alpha(reading *readings, int nReadings)
{
	/* Compute the error probability from ASCII characters IN LOG SCALE */
	double pr[94];
	for (int i = 33; i < 127; i++)
		pr[i - 33] = 1 - pow(10, -((i - 33) / 10.));

	/* Compute the expected number of error free reads in each sequence */
	double *y = malloc(nReadings * sizeof(double));

	for (int i = 0; i < nReadings; i++) {
		y[i] = 1;

		/* Mean error probability of the sequence IN LOG SCALE */
		for (int j = 0; j < readings[i].n; j++)
			for (int k = 33; k < 127; k++)
				if ((int)(readings[i].scores[j]) == k)
					y[i] += (1 - pr[k - 33]);
	}

	/* Compute the MLE for the alpha parameter */
	double sum = 0;
	for (int i = 0; i < nReadings; i++)
		sum += log(y[i]); /* divide by 1 */

	double alpha = 1 + nReadings / sum;

	free(y);
	return (alpha);
}

double score_function(double alpha, void *args)
{
	int *countsUnique = ((int **)args)[0];
	int nUnique = *((int**)args)[1];

	double sum = 0;
	for (int i = 0; i < nUnique; i++)
		sum += log(countsUnique[i]);

	return (-nUnique * digamma(alpha) + sum);
}

double bic_score_function(double alpha, void *args)
{
	int *countsUnique = ((int **)args)[0];
	int nUnique = *((int**)args)[1];
	int xmin = *((int**)args)[2];

	double sum = 0;
	for (int i = 0; i < nUnique; i++)
		sum += log(countsUnique[i] / xmin);

	return (-nUnique * digamma(alpha) + sum);
}

