/**
 * @file bisection.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Implementation of the binary search algorithm.
 */

#include "interface.h"
#include "main.h"
#include <stdio.h>
#include <math.h>

#define SIGN(x) ((x > 0) - (x < 0))

/**
 * Finds a root of a function using the bisection method.
 * @param f A continuous function of one variable.
 * @param a The left bound of the search space.
 * @param b The right bound of the search space.
 * @param target The target value of the function (may be zero for a root).
 * @param epsilon Absolute error allowed (precision).
 * @return The value of x where f(x) is zero assuming that x \in [a, b] and
 * sign(f(a)) != sign(f(b)).
 */
double bisection(double (*f)(double, void*), void *args, double a, double b,
		double target, double epsilon)
{
	if (SIGN((*f)(a, args) - target) == SIGN((*f)(b, args) - target))
		fatal("f(a) and (b) cannot have the same sign");

	double x = (a + b) / 2;

	while (fabs((*f)(x, args) - target) > epsilon &&
			(b - a) / 2 > epsilon) {

#ifdef DEBUG
		printf(
				"DEBUG: a = % .15e b = % .15e x = % .15e, f(x) = % .15e.\n",
				a, b, x, (*f)(x, args) - target);
#endif

		if (SIGN((*f)(x, args) - target) ==
				SIGN((*f)(a, args) - target)) {
			a = x;
		} else {
			b = x;
		}
		x = (a + b) / 2;
	}

#ifdef DEBUG
	printf(
			"DEBUG: a = % .15e b = % .15e x = % .15e, f(x) = % .15e.\n",
			a, b, x, f(x, args) - target);
#endif

	return (x);
}
