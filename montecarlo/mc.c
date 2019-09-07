/*
 * @file mc.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION: This code 
 *
 * Compile as:
 * gcc -o mc mc.c -Wall -pedantic -lm -lRmath
 *
 * Usage:
 *	./mc -n <n-value> -m <M-value>
 *
 */

#define MATHLIB_STANDALONE 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <Rmath.h>
#include <limits.h>
#include <time.h>

/* Declare functions */
void swap(double *x, double *y);
int choose_pivot(int i, int j );
void quicksort(double list[], int m, int n);
double mean(int ssize);
double order(int n, int ssize);
unsigned int time_to_seed(time_t t);

int main(int argc, char **argv)
{
	int n, M;

	/* Read user-input */
    int opt;
    while ((opt = getopt(argc, argv, "n:m:")) != -1) {
        switch (opt) {
            case 'n':
            	sscanf(optarg, "%d", &n);
                printf("n = %d\n", n);
            	break;
            case 'm':
            	sscanf(optarg, "%d", &M);
                printf("m = %d\n", M);
            	break;
            case '?':
                printf("FATAL: unknown option.\\n");
                return (EXIT_FAILURE);
        }
    }

    int ssize = 2*n - 1;		/* define sample size */
    double temp = 0;
    double mom1, mom2;

    /* set random seeds */
	srand(time_to_seed(time(NULL)));
    set_seed(rand(), rand());

    /* Monte Carlo approximation of the first moment */
    for (int m = 0; m < M; m++) {
    	temp += mean(ssize);
    }
    mom1 = temp / M;
    printf("mom1 = %lf\n", mom1);

    /* Monte Carlo approximation of the second moment */
    temp = 0;
    for (int m = 0; m < M; m++) {
    	temp += pow(mean(ssize), 2);
    }
    mom2 = temp / M;
    printf("mom2 = %lf\n", mom2);

    /* Monte Carlo approximation of Var(X_(n)) by contol variates */
    double varXn;
    varXn = mom2 - mom1 * mom1;

    printf("The Monte Carlo approximation of Var(X_(n)) using \n control of variates is computed as %lf\n", varXn);


    /*Compare with the simpler way of Monte Carlo approximating */
    temp = 0;
    double varXn2;
    for (int m = 0; m < M; m++) {
    	temp += pow((order(n, ssize)), 2);
    }
    varXn2 = temp / M;

    printf("The Monte Carlo approximation of Var(X_(n)) using \n the simpler way is computed as %lf\n", varXn2);

    return(EXIT_SUCCESS);
}



/***********************************************************************/
/********************** Supplementary functions ************************/
/***********************************************************************/

/* Compute the mean of a sample from rnorm(0,1), where the 
 * sample is given as input: ssize
 * 
 * @param ssize integer sample size
 * @return mean double
 * @auhtor SSGL
 */
double mean(int ssize)
{
	double temp = 0;
	double mean;

	/* sample from rnorm(0,1) */
	for (int i = 0; i < ssize; i++) {
		temp += rnorm(0,1);
	}

	mean = temp / (double) ssize;
	return(mean);
}


/* Sample from rnorm(0, 1) and return nth ordered statistic, given
 * sample size = 2n - 1. Quick sort algorithm of complexity O(logn)
 * was used to find the nth ordered statistic.
 * 
 * @param ssize integer sample size
 * @param n integer 
 * @return nth double ordered statistic
 * @auhtor SSGL
 */
double order(int n, int ssize)
{
	double sample[ssize];

	for (int i = 0; i < ssize; i++) {
		sample[i] = rnorm(0,1);
	}
	/* sort the list using quicksort algorithm */
    quicksort(sample, 0, ssize-1);
    return(sample[n-1]);
}


/* Generate random unsigned int seed from time_t object.
 *
 * @param t time_t object
 * @return unsigned int seed
 * @author Dr. Karin Dorman
 */
unsigned int time_to_seed(time_t t)
{
        unsigned int ret = 0;
        for(size_t i=0; i<sizeof(t); i++) {
                char c = (char) (t >> i*8);
                ret = ret * (UCHAR_MAX + 2U) + c;
        }
        return ret;
} /* time_to_seed */


/* Given array of elements, sort them in ascending order.
 *
 * @authors: zentut<>
 * @credits: http://www.zentut.com/c-tutorial/c-quicksort-algorithm/
 */
void swap(double *x, double *y)
{
    double temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
int choose_pivot(int i, int j )
{
    return((i+j) /2);
}
 
void quicksort(double list[], int m, int n)
{
    double key;
    int i, j, k;
    if( m < n)
    {
        k = choose_pivot(m,n);
        swap(&list[m],&list[k]);
        key = list[m];
        i = m+1;
        j = n;
        while(i <= j)
        {
            while((i <= n) && (list[i] <= key))
                i++;
            while((j >= m) && (list[j] > key))
                j--;
            if( i < j)
                swap(&list[i],&list[j]);
        }
        /* swap two elements */
        swap(&list[m],&list[j]);
 
        /* recursively sort the lesser list */
        quicksort(list,m,j-1);
        quicksort(list,j+1,n);
    }
}