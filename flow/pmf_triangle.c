/**
 * @file pmf_triangle.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Implementation of the Poisson Binomial probability mass function based on
 * a generalization of Pascal's triangle for binomial coefficients. Should
 * be more stable.
 *
 * Profiled with gprof based on
 * https://web.eecs.umich.edu/~sugih/pointers/gprof_quick.html
 */

#include "interface.h"

/**
 * Probability mass function of a Poisson Binomial: implementation based
 * on a generalization of Pascal's triangle.
 *
 * @param n Number of trials.
 * @param k Number of successes (k <= n).
 * @param p An n-sized array with the success probability for each n trial.
 * @return The probability mass function evaluated for n and k.
 */
myfloat pmf_triangle(int n, int k, myfloat *p)
{
	myfloat pnk = 1; /* target probability */

	/* Algorithm branches out by cases */
	if (k == 0) {
		for (int t = 0; t < n; t++)
			pnk *= 1 - p[t];
	} else if (k == n) {
		for (int t = 0; t < n; t++)
			pnk *= p[t];
	} else {
		/* NOTE: The algorithm is a bit involved and code looks
		 * necessarily complicated. Kernel Style makes things
		 * worse, especially due to large tabs.
		 * 1. There are two probability arrays for odd/even trials.
		 * 2. There's a bound to avoid unnecessary calculations.
		 */
		int max_length = (n + 2) / 2;
		unsigned int left_bound = n - k;
		myfloat p_odd[max_length];
		myfloat p_even[max_length];

		/* n in {1, 2} */
		p_odd[0] = 1 - p[0];
		p_odd[1] = p[0];

		/*
		 * For n > 2, the best we could design is O(n^2)
		 */
		for (int i = 2; i <= n; i++) {
			unsigned int l = 0, m = 0;

			/* for j == 0 */
			if (i <= left_bound) {
				if (i & 1) {
					p_odd[l] = (1 - p[i - 1]) * p_even[m];
				} else {
					p_even[l] = (1 - p[i - 1]) * p_odd[m];
				}

				l++;
			}

			/* for j > 0 */
			for (int j = 0; j <= k; j++) {
				/* Skip as much as we can */
				if (i - j > left_bound)
					continue;

				if (j == i) {
					if (i & 1) {
						p_odd[l] = p[i - 1]
								* p_even[m];
					} else {
						p_even[l] = p[i - 1]
								* p_odd[m];
					}

				} else {
					if (i & 1) {
						p_odd[l] =
								p[i - 1]
										*
										p_even[m]
										+
										(1
												- p[i
														- 1])
												* p_even[m
														+ 1];
					} else {
						p_even[l] =
								p[i - 1]
										* p_odd[m]
										+ (1
												- p[i
														- 1])
												* p_odd[m
														+ 1];
					}
					m++;
				}
				l++;
			}
		}

		if (n & 1) {
			pnk = p_odd[0];
		} else {
			pnk = p_even[0];
		}
	}

	return (pnk);
}
