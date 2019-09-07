/**
 * @file pmf.h
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Various implementations of the probability mass function for a Poisson
 * Binomial random variable.
 */

#ifndef PMF_H_
#define PMF_H_

myfloat pmf_naive(int n, int k, myfloat *p);
myfloat pmf_recursion(int n, int k, myfloat *p);
myfloat pmf_triangle(int n, int k, myfloat *p);

#endif /* PMF_H_ */
