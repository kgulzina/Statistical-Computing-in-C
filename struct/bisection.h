/*
 * bisection.h
 *
 *  Created on: Mar 25, 2019
 *      Author: luis
 */

#ifndef BISECTION_H_
#define BISECTION_H_

double bisection(double (*f)(double, void*), void *args, double a, double b,
		double target, double epsilon);

#endif /* BISECTION_H_ */