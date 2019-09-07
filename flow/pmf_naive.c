/**
 * @file pmf_naive.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Naive implementation of the Poisson Binomial probability mass function.
 *
 * Borrows heavily from  https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
 */

#include "interface.h"

void pmf_naive_innerprod(myfloat *p, int k, int currentIndex,
		myfloat currentComb[], int start, int end, myfloat *pnk);

/**
 * Run an iteration in the recursive step: fill an empty slot in the
 * combination of probabilities, or compute the inner product.
 *
 * @param p An n-sized array with the success probability for each n trial.
 * @param k Number of successes.
 * @param currentIndex Position of the last non-free element in the current
 * combination of success probabilities.
 * @param currentComb A k-sized array with the indexes of the success
 * probabilities in the current combination.
 * @param start Index of the starting position of the probability array
 * (varies per iteration).
 * @param end Index of the last position of the probability array
 * (varies per iteration).
 * @param pnk Variable where the target probability should be written to.
 *
 * @note The implementation of the combinatoric aspects heavily borrows from
 * Method 1 as described in https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
 */
void pmf_naive_innerprod(myfloat *p, int k, int currentIndex,
		myfloat currentComb[], int start, int end, myfloat *pnk)
{
	/*
	 * CASE 1
	 * Condition: Current combination has nonempty slots.
	 * Action: Calculate the inner product for this particular combination.
	 */
	if (currentIndex == k) {
		myfloat innerProd = 1;

		for (int i = 0; i < k; i++)
			innerProd *= (currentComb[i]) / (1 - currentComb[i]);

		(*pnk) += innerProd;
		return;
	}

	/*
	 * CASE 2:
	 * We still have to build the combination index (i.e. there is
	 * at least one empty slot). Fill next slot.
	 */
	for (int i = start; i < end && end - i >= k - currentIndex; i++) {
		currentComb[currentIndex] = p[i];
		pmf_naive_innerprod(p, k, currentIndex + 1, currentComb, i + 1,
				end, pnk);
	}
}

/**
 * Probability mass function of a Poisson Binomial: naive implementation.
 * Beware, it's slow.
 *
 * @param n Number of trials.
 * @param k Number of successes (k <= n).
 * @param p An n-sized array with the success probability for each n trial.
 * @return The probability mass function evaluated for n and k.
 */
myfloat pmf_naive(int n, int k, myfloat *p)
{
	myfloat currentComb[k]; /* temporary array with current indexes */
	myfloat pnk = 0; /* target probability */

	/* Compute the inner product */
	pmf_naive_innerprod(p, k, 0, currentComb, 0, n, &pnk);

	/* Compute the outer product */
	for (int i = 0; i < n; i++)
		pnk *= (1 - currentComb[i]);

	return pnk;
}
