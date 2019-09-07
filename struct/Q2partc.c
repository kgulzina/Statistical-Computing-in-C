/**
 * @file Q2partc.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Implementation of the binary search algorithm to find the 
 * MLE of alpha for a sequence in FASTQ file given xmin is
 * not pre-defined to maximize the BIC.
 * Compile as:
 *     gcc -Wall -pedantic -o Q2partc Q2partc.c -lm -lRmath
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MATHLIB_STANDALONE
#include "Rmath.h"

#define LENGTH 256  /* Length of the sequence */

#define DEBUG 

/* Declarations of functions to be used */
void fatal(char *message);
double score (double alpha);				//score function
double bisection(double (fun)(double), double a, double b, double target, double epsilon);


/* Main Function */
int main(int argc, char **argv)
{
#ifdef DEBUG
	printf("DEBUG: Debug mode was set by the gods at compile time.\n");
#endif

	/* For a and b, we consider the min and max of FASTQ file
	sequence. We have found them from the R program.
	*/ 
	double out = bisection(score, 28329, 7344124, 0, 1E-16);

	printf("root: %f\n", out);

	return EXIT_SUCCESS;
}


/***************************************************************************/
/************************* Supplementary Functions *************************/
/***************************************************************************/

/* Function which needs to be maximized to find the root fun(alpha)=0 
 *  ==> alpha_MLE */

double score (double alpha)
{
	FILE *fp;
	int i;	
	
	int xvec[LENGTH*LENGTH];
	int xmin[LENGTH*LENGTH];	

	fp = fopen("sequencepartc.txt", "r");

	if (fp == NULL){
		printf("Error: can't open file.\n");
		return 1;
	}else{
		i = 0;
	
		while (!feof(fp)) {
			/* "cat sequencepartc.txt" shows space as a separator of columns.*/
			fscanf(fp, "%d %d", &xvec[i], &xmin[i]);
			i++;
		}
	}

	fclose(fp);

	/* score function itself */
	double SUM = 0;
	for (i = 0; i < LENGTH*LENGTH; i++)
		SUM += log(xvec[i]/xmin[i]);	
	return (LENGTH * digamma(alpha) + SUM);

}


/**
 * Finds a root of a function using the Bisection method.
 * @param fun: A continuous function of one variable.
 * @param a: The left bound of the search space.
 * @param b: The right bound of the search space.
 * @param target: The target value of the function (set to zero for a root).
 * @param epsilon: Absolute error allowed (precision).
 * @return The value of x where f(x) is zero assuming that x \in [a, b] and
 * sign(fun(a)) != sign(fun(b)).
 */
double bisection(double (fun)(double), double a, double b, double target, double epsilon)
{
	if (sign((fun)(a) - target) == sign((fun)(b) - target))
		printf("fun(a) and fun(b) cannot have the same sign");

	double x = (a + b) / 2;

	while (fabs((fun)(x) - target) > epsilon &&
			(b - a) / 2 > epsilon) {

#ifdef DEBUG
		printf(
				"DEBUG: a = % f b = % f x = % .15e, fun(x) = % f.\n",
				a, b, x, (fun)(x) - target);
#endif

		if (sign((fun)(x) - target) ==
				sign((fun)(a) - target)) {
			a = x;
		} else {
			b = x;
		}
		x = (a + b) / 2;
	}

#ifdef DEBUG
	printf(
			"DEBUG: a = % f b = % f x = % f, fun(x) = % f.\n",
			a, b, x, fun(x) - target);
#endif

	return (x);
}


/**
 * Print a fatal error message and exit with failure.
 * @param message Error message.
 */
void fatal(char *message)
{
	fprintf(stderr, "FATAL ERROR: %s.\n", message);

	exit(EXIT_FAILURE);
}


