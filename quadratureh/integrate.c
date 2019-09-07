/*
 * @file integrate.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION: This code tames two integrals using Gaussian-Laguerre
 * quadrature method. Abscissa and corresponding weights for different
 * n-degree Laguerre polynomials were obtained using function in R-package 
 * "gaussguad": laguerre.quadrature.rules(n). 
 * Table with abscissa values and weights are read in into this code with
 * max(n) = 1000. Code runs until max(n) is reached or until convergence
 * is reached. Code takes in user-input values as alpha, x_min and
 * outputs approximate result for 1st or 2nd integral. User can choose 
 * which integral to compute by entering -i value = {1,2}.
 *
 * Compile as:
 * gcc -o integrate integrate.c -Wall -pedantic -lm
 *
 * Usage:
 *	./integrate -a <alpha> -x <x_min> -i <which integal>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* Declare functions */
double f1(double t, double alpha, int x_min);
double f2(double t, double alpha, int x_min);

int main(int argc, char **argv)
{
	double alpha;
	int x_min;						/* integer, since it is count number */
	int which;						/* which integral to tame */

	/* Read user-input */
    int opt;
    while ((opt = getopt(argc, argv, "a:x:i:")) != -1) {
        switch (opt) {
            case 'a':
            	sscanf(optarg, "%lf", &alpha);
                printf("alpha = %lf\n", alpha);
            	break;
            case 'x':
            	sscanf(optarg, "%d", &x_min);
                printf("x_min = %d\n", x_min);
            	break;
            case 'i':
            	sscanf(optarg, "%d", &which);
                printf("which integral = %d\n", which);
            	break;
            case '?':
                printf("FATAL: unknown option.\\n");
                return (EXIT_FAILURE);
        }
    }

    /* Check user-inputs */
    if (alpha <= 0) {
        printf("FATAL: alpha must be bigger than 0.\n");
        return (EXIT_FAILURE);
    }
    if (x_min < 1) {
        printf("FATAL: x_min must be bigger than or equal to 1.\n");
        return (EXIT_FAILURE);
    }
    if (which != 1 && which != 2) {
    	printf("FATAL: Enter i is out of range.\n");
        return (EXIT_FAILURE);
    }

    /* Read in abscissa and weights of Laguerre polynomials */	
    FILE *fp;
    fp = fopen("laguerre.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open the file!\n");
        return EXIT_FAILURE;
    }

    double laguerre[500500][2];		/* max(n) = 1000 */
    for (int row = 0; row < 500500; row++) {
    	for (int col = 0; col < 2; col++) {
    		fscanf(fp, "%lf", &laguerre[row][col]);
    	}
    }
    fclose(fp);

    //to check data is read correctly:
    printf("%lf, %lf\n", laguerre[8][0], laguerre[13][1]);

    /* Tame the integrals */
    double epsilon = 0.000001;
    double temp;
    double res_prev;
    double res_next = 580;
    int count, start, stop;

    if (which == 1) {
    	for (int n = 1; n < 1001; n++) {
    		temp = 0;
    		start = (n-1)*n / 2;	/* starting row in Laguerre table */
    		stop = n*(n+1) / 2;		/* stopping row in Laguerre table */
    		for (int i = start; i < stop; i++) {
    			temp += laguerre[i][1] * f1(laguerre[i][0], alpha, x_min);
    		}
    		res_prev = res_next;
    		res_next = temp;
            printf("%lf\n", res_next);
    		if (fabs(res_prev - res_next) <= epsilon)
    		{
    			count = n;
    			break;
    		}
    		count = n;
    	}
    } else {
    	for (int n = 1; n < 1001; n++) {
    		temp = 0;
    		start = (n-1)*n / 2;	/* starting row in Laguerre table */
    		stop = n*(n+1) / 2;		/* stopping row in Laguerre table */
    		for (int i = start; i < stop; i++) {
    			temp += laguerre[i][1] * f2(laguerre[i][0], alpha, x_min);
    		}
    		res_prev = res_next;
    		res_next = temp;
            printf("%lf\n", res_next);
    		if (fabs(res_prev - res_next) <= epsilon)
    		{
    			count = n;
    			break;
    		}
    		count = n;
    	}
    }

    /* Print the result */
    printf("Approximate value of the integral = %lf, is reached at %d\n", res_next, count);

	return(EXIT_SUCCESS);
}

/***********************************************************************/
/********************** Supplementary functions ************************/
/***********************************************************************/
/* integrand in the first integral */
double f1(double t, double alpha, int x_min) 
{
	double res;
    if (t < 0.1) {
        res = 6 * pow(t, alpha-2) * exp((1-x_min)*t) / (t*t - 3*t + 6);
    } else {
        res = pow(t, alpha-1) * exp((1-x_min)*t) / (1-exp(-t));
    }
	
	return(res);
}

/* integrand in the second integral */
double f2(double t, double alpha, int x_min)
{
	double res;
    if (t < 0.1) {
        res = 6 * pow(t, alpha-2) * exp((1-x_min)*t) * log(t) / (t*t - 3*t + 6);
    } else {
        res = pow(t, alpha-1) * exp((1-x_min)*t) * log(t) / (1-exp(-t));
    }
	return(res);
}
