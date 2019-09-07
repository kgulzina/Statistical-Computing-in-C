/**
 * @file main.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Computes the probability mass function of a Poisson Binomial distribution
 * for a number of successes k, given a fixed number of independent Bernoulli
 * trials n and an n-sized vector of probabilities fed to stdin.
 *
 * Usage:
 *	./poisbin -n <int> -k <int> -m [nrt] < cat p.txt
 *	where n = naive (default), r = recursive, t = Pascal's triangle.
 *
 * One-step example:
 *	./qbuild; ./run_check
 *
 * All in one custom run:
 *	rm poisbin; gcc -Wall -pedantic -o poisbin main.c interface.c \
 *	pmf_naive.c pmf_recursion.c pmf_triangle.c -lm; \
 *	cat stat580_hw3.txt | ./poisbin -n 7 -k 3
 *
 * Compile as:
 *	rm poisbin; gcc -Wall -pedantic -o poisbin main.c interface.c \
 *	pmf_naive.c pmf_recursion.c pmf_triangle.c -lm; \
 *
 * Run as:
 *	cat stat580_hw3.txt | ./poisbin -n 7 -k 3 -m
 *	./poisbin -n 7 -k 3 < stat580_hw3.txt
 *
 * Note re: LKCS in pmf_naive.c:15
 * https://groups.google.com/forum/#!topic/fa.linux.kernel/kDd31-wLOeY
 */

#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "pmf.h"

int main(int argc, char **argv)
{
	int n = -1;
	int k = -1;
	char m = 'n';
	myfloat p[MAX_N];
	int len = 0;

	read_argv(argc, argv, &n, &k, &m);
	read_data(n, MAX_N, p, &len);
	/* print_data(n, p); */

	switch (m) {
	case 'n':
		printf(
				"Naive     p(n =% 6i | k =% 6i) = %"
				FLOAT_FMT_OUT").\n",
				n, k, pmf_triangle(n, k, p));
		break;
	case 'r':
		printf(
				"Recursion p(n =% 6i | k =% 6i) = %"
				FLOAT_FMT_OUT").\n",
				n, k, pmf_recursion(n, k, p));
		break;
	case 't':
		printf(
				"Triangle  p(n =% 6i | k =% 6i) = %"
				FLOAT_FMT_OUT").\n",
				n, k, pmf_triangle(n, k, p));
		break;
	default:
		fatal("unknown method");
	}

	return EXIT_SUCCESS;
}
