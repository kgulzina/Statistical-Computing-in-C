/**
 * @file main.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION
 *
 * Usage:
 *	Rscript run.R
 *
 * Compile as:
 * 	For R:
 * 		R CMD SHLIB main.c -o main.o
 *
 * 	Standalone:
 *		rm a.out; gcc *.c -DSTANDALONE -pedantic -Wall -lm \\
 *		-I/usr/share/R/include; ./a.out;
 *
 */

#include <stdio.h>
#include <stdlib.h> /* exit codes */
#include <Rinternals.h> /* R */
#include <math.h> /* fabs */
#include "main.h"

double pgf(double x);
double d_pgf(double x);
double newton(double s, double epsilon, int maxiter);

static double coeff[11] = {0.4982, 0.2103, 0.1270, 0.0730, 0.0418, 0.0241,
		0.0132, 0.0069, 0.0035, 0.0015, 0.0005};

/**
 * Probability generating function of the branching process.
 * @param x Number of offspring.
 * @return The corresponding probability.
 */
double pgf(double x)
{
	/* Horner's method */
	double sum = coeff[10];
	for (int i = 9; i >= 0; --i)
		sum = x * sum + coeff[i];
	return sum;
} /* pgf */

/**
 * First derivative of `pgf`.
 * @param x Value.
 * @return First derivatived evaluated at `x`.
 */
double d_pgf(double x)
{
	double sum = coeff[10];
	double sumd = coeff[10];
	sum = x * sum + coeff[9];
	for (int i = 8; i >= 0; --i) {
		sumd = x * sumd + sum;
		sum = x * sum + coeff[i];
	}
	return sumd;
} /* d_pgf */

/**
 * Find the extinction probability by Newton-Raphson.
 * @param initial Initial value.
 * @param epsilon Convergence cutoff.
 * @param maxiter Maximum number of iterations allowed.
 * @return The extinction probability.
 */
double newton(double s, double epsilon, int maxiter)
{
	/* Let's keep things reasonable */
	if ((s < 0) | (s > 1))
		s = 0;

	double s_next, diff;

	do {
		s_next = s + (pgf(s) - s) / (1 - d_pgf(s));
		diff = fabs(s_next - s);
		s = s_next;
#ifdef STANDALONE
		printf("DEBUG: s =% .6e (diff =% .6e) \n", s, diff);
#endif
	} while ((diff > epsilon) & (maxiter-- > 0));

	return (s);
}

#ifndef STANDALONE
SEXP rnewton(SEXP s_r, SEXP epsilon_r, SEXP maxiter_r)
{
	/* Check inputs */
	if (!isReal(s_r))
		error("[ERROR] First argument (initial) must be double!\n");

	if (!isReal(epsilon_r))
		error("[ERROR] Second argument (epsilon) must be double!\n");

	if (!isInteger(maxiter_r))
		error("[ERROR] Third argument (maxiter) must be integer!\n");

	/* Reformat R -> C */
	double s, epsilon;
	int maxiter;
	s = *REAL(s_r);
	epsilon = *REAL(epsilon_r);
	maxiter = *INTEGER(maxiter_r);

	/* Run computations finally ;_; */
	s = newton(s, epsilon, maxiter);

	/* Reformat C -> R */
	SEXP out_r = PROTECT(ScalarReal(s));
	UNPROTECT(1);
	return out_r;
}
#endif /* NOT STANDALONE */

#ifdef STANDALONE
int main(int argc, char **argv)
{

#ifdef STANDALONE
	printf("DEBUG: Somebody set standalone mode on, woohoo!\n");
#endif

	double out = newton(0, 1e-6, 1000);

	printf("Done!  s =% .6e\n", out);

	return EXIT_SUCCESS; /* @suppress("Return with parenthesis") */
}
#endif /* STANDALONE */
