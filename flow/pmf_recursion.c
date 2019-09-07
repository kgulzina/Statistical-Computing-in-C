/**
 * @file pmf_recursion.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Recursive implementation for the Poisson Binomial probability mass function.
 *
 * Profiled with gprof based on
 * https://web.eecs.umich.edu/~sugih/pointers/gprof_quick.html
 */

#include <math.h>
#include "interface.h"

/**
 * Probability mass function of a Poisson Binomial: recursive.
 * Beware, may be unstable.
 *
 * @param n Number of trials.
 * @param k Number of successes (k <= n).
 * @param p An n-sized array with the success probability for each n trial.
 * @return The probability mass function evaluated for n and k.
 */
myfloat pmf_recursion(int n, int k, myfloat *p)
{
	myfloat pnk = 1; /* target probability */

	if (k == 0) {
		for (int i = 0; i < n; i++)
			pnk *= (1 - p[i]);
	} else {
		myfloat temp_sum;
		myfloat temp_prob[k + 1];
		myfloat t = 0;
		myfloat p_odd[n];
		myfloat p_odd_powered[n];

		/* Precomputations */

		/* Note: we considered storing 1-p as an intermediate quantity,
		 * but we believed it was not worth the writing/reading time.
		 */
		temp_prob[0] = 1;
		for (int j = 0; j < n; j++) {
			temp_prob[0] *= 1 - p[j];
			p_odd[j] = p[j] / (1 - p[j]);
			p_odd_powered[j] = p_odd[j]; /* p_odd^1 */
			t += p_odd_powered[j]; /* t(1) */
		}

		for (int m = 1; m <= k; m++) {
			temp_sum = 0;

			/* Note: m corresponds to the i from 1 to k subscript
			 * in the first equation of 1(b)
			 */
			for (int i = m; i >= 1; i--) {
				t = 0;
				for (int j = 0; j < n; j++)
					t += pow(p_odd[j], i);

				/* Note: C's pow is not the best option since
				 * it is a general case for a double base and
				 * a double exponent. In our case, we could
				 * achieve better optimization using a loop.
				 * Not implemented because of we're too close
				 * to the deadline, sorry =/
				 */
				if (i & 1) { /* if odd, the term is positive */
					temp_sum += temp_prob[m - i] * t / m;
				} else {
					temp_sum -= temp_prob[m - i] * t / m;
				}
			}
			temp_prob[m] = temp_sum;
		}
		pnk = temp_prob[k];
	}

	return pnk;
}
