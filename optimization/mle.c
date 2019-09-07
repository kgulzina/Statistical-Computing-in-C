/**
 * @file mle.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION
 *
 * Compile as:
 *	gcc -o mle mle.c -Wall -pedantic -lm
 *
 * Usage:
 *	./mle -m n (or f) -i 0.1179135 -j 0.5508362 -e 0.000001
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <unistd.h>

/* Declare functions */
int factorial(int n);
double *fisher(double init[], double epsilon, double x[][2], int y[], int n);
double *fisher_update(double *theta, double x[][2], int y[], int n, int back);
//double likelihood(double *theta, double x[][2], int y[], int n);
//double log_likelihood(double *theta, double x[][2], int y[], int n);
double *newton(double init[], double x[][2], int y[], int n, double epsilon);
double *newton_update(double *theta, double x[][2], int y[], int n, int back);

int main(int argc, char **argv)
{
    char *method = NULL;			/* which method to use */
    double init1;
    double init2;
    double epsilon;		           /* convergence criteria */
    
    /* Read user-input */
    int opt;
    while ((opt = getopt(argc, argv, "m:i:j:e:")) != -1) {
        switch (opt) {
            case 'm':
                method = optarg;
                printf("method = %s\n", method);
                break;
            case 'i':
                sscanf(optarg, "%lf", &init1);
                printf("initial beta1 = %lf\n", init1);
                break;
            case 'j':
            	sscanf(optarg, "%lf", &init2);
                printf("initial beta2 = %lf\n", init2);
            	break;
            case 'e':
            	sscanf(optarg, "%lf", &epsilon);
                printf("epsilon = %lf\n", epsilon);
            	break;
            case '?':
                printf("FATAL: unknown option.\\n");
                return (EXIT_FAILURE);
        }
    }
    
    if (epsilon < 0) {
        printf("FATAL: epsilon must be bigger than or equal to 0.\n");
        return (EXIT_FAILURE);
    }
    
    /* Read data from file */
    FILE *fp;

    fp = fopen("oil.Rtxt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open the file!\n");
        return EXIT_FAILURE;
    }

    /* Read in and discard first line */
    char header[40];            /* to skip header */
    double x[26][2];          	/* to store covariates */
    int y[26];					/* to store response */
    int year[26];               /* to store year */

    fgets(header, sizeof(header), fp);
    for (int row = 0; row < 26; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (col == 0) {
                fscanf(fp, "%d", &year[row]);
            } else if (col == 1) {
                fscanf(fp, "%d", &y[row]);
            } else if (col == 2) {
                fscanf(fp, "%lf", &x[row][0]);
            } else {
            	fscanf(fp, "%lf", &x[row][1]);
            }
        }
    }
    fclose(fp); 

    //to check if data is read correctly
    printf("%lf, %lf, %lf\n", x[0][0], x[1][0], x[25][1]);

    /* Find MLE's with respect to chosen method given initial values */
    double *estimates = malloc(2 * sizeof(double));
    estimates[0] = init1;
    estimates[1] = init2;

    if (strcmp(method,"n") == 0) {
    	/* update estimates */
    	estimates = newton(estimates, x, y, 26, epsilon);
    	printf("MLE estimates of beta1 and beta2 are: %lf, %lf respectively\n", estimates[0], estimates[1]);
    } else {
    	estimates = fisher(estimates, epsilon, x, y, 26);
    	printf("MLE estimates of beta1 and beta2 are: %lf, %lf respectively\n", estimates[0], estimates[1]);
    }
    return(EXIT_SUCCESS);
}




/***********************************************************************/
/********************** Supplementary functions ************************/
/***********************************************************************/
int factorial(int n)
{
	if (n < 0) {
		printf("Cannot compute factorial of negative number!");
	}
	if (n == 0 || n == 1) {
		return(1);
	} else {
		int res = 1;
		while (n > 1) {
			res *= n;
			n--;
		}
		return(res);
	}
}


double *fisher(double init[], double epsilon, double x[][2], int y[], int n)
{
	/* Find MLE's of beta1 and beta2 parameters */
    /* When n = 0, set initial values for estimates */
    int i = 0;
    double *theta = malloc(2 * sizeof(double));
    double *theta_next = malloc(2 * sizeof(double));
    theta_next[0] = init[0];
    theta_next[1] = init[1];
    
    do {
        i++;
        theta = theta_next;
        theta_next = fisher_update(theta, x, y, n, 1);
        printf("Iteration n = %d: beta1 = %lf, beta2 = %lf\n", i, theta_next[0], theta_next[1]);
    } while (fabs(theta_next[0] - theta[0]) > epsilon || fabs(theta_next[1] - theta[1]) > epsilon);
    return(theta);
}


double *fisher_update(double *theta, double x[][2], int y[], int n, int back)
{
    double dl2[3] = {0, 0, 0};                      /* to store -dl^2(theta)*/
    double dl1[2] = {0, 0};                         /* to store dl(theta)' */
    double *update = malloc(2 * sizeof(double));
    double temp;

    for (int i = 0; i < n; i++) {
        /* Update all entries at once */
        temp = 1 / (theta[0]*x[i][0] + theta[1]*x[i][1]);                  
        dl2[0] += x[i][0]*x[i][0]*temp;
        dl2[1] += x[i][0]*x[i][1]*temp;
        dl2[2] += x[i][1]*x[i][1]*temp;
        dl1[0] += x[i][0]*y[i]*temp - x[i][0];
        dl1[1] += x[i][1]*y[i]*temp - x[i][1]; 
    }

    /* Get inverse of -dl^2(theta) */
    double invDet = 1 / (dl2[0]*dl2[2] - dl2[1]*dl2[1]);
     
    /* Update theta */
    if (back == 0) {
        //backtracking
        update[0] = theta[0] + (invDet*dl2[2]*dl1[0] - invDet*dl2[1]*dl1[1])/2;
        update[1] = theta[1] + (invDet*dl2[0]*dl1[1] - invDet*dl2[1]*dl1[0])/2;
    } else {
        update[0] = theta[0] + (invDet*dl2[2]*dl1[0] - invDet*dl2[1]*dl1[1]);
        update[1] = theta[1] + (invDet*dl2[0]*dl1[1] - invDet*dl2[1]*dl1[0]);
    }
    return(update);
}


//no need for likelihood functions for now
/*double likelihood(double *theta, double x[][2], int y[], int n)
{
    double res, temp, sum = 0;
    double prod1 = 1, prod2 = 1;
    for (int i = 0; i < n; i++) {
        temp = theta[0]*x[i][0] + theta[1]*x[i][1];
        sum -= temp;
        prod1 *= pow(temp, y[i]);
        prod2 *= factorial(y[i]);
    }
    res = exp(sum) * prod1 / prod2;
    return(res);
}*/

/*double log_likelihood(double *theta, double x[][2], int y[], int n)
{
	double temp, res = 0;
	for (int i = 0; i < n; i++) {
		temp = theta[0]*x[i][0] + theta[1]*x[i][1];
		res += (-temp + y[i]*log(temp) - log(factorial(y[i]))); 
	}
	return(res);
}*/


double *newton(double init[], double x[][2], int y[], int n, double epsilon)
{
	/* Find MLE's of beta1 and beta2 parameters */
    /* When n = 0, set initial values for estimates */
    int i = 0;
    double *theta = malloc(2 * sizeof(double));
    double *theta_next = malloc(2 * sizeof(double));
    theta_next[0] = init[0];
    theta_next[1] = init[1];
    
    do {
        i++;
        theta = theta_next;
        theta_next = newton_update(theta, x, y, n, 1);
        printf("Iteration n = %d: beta1 = %lf, beta2 = %lf\n", i, theta_next[0], theta_next[1]);
    } while (fabs(theta_next[0] - theta[0]) > epsilon || fabs(theta_next[1] - theta[1]) > epsilon);
    return(theta);    
}


double *newton_update(double *theta, double x[][2], int y[], int n, int back)
{
	double dl2[3] = {0, 0, 0};						/* to store -dl^2(theta)*/
	double dl1[2] = {0, 0};							/* to store dl(theta)' */
	double *update = malloc(2 * sizeof(double));
	double temp1, temp2;

	for (int i = 0; i < n; i++) {
		/* Update all entries at once */
		temp1 = 1 / (theta[0]*x[i][0] + theta[1]*x[i][1]);
		temp2 = temp1 * temp1;					
		dl2[0] += x[i][0]*x[i][0]*y[i]*temp2;
		dl2[1] += x[i][0]*x[i][1]*y[i]*temp2;
		dl2[2] += x[i][1]*x[i][1]*y[i]*temp2;
		dl1[0] += x[i][0]*y[i]*temp1 - x[i][0];
		dl1[1] += x[i][1]*y[i]*temp1 - x[i][1]; 
	}

	/* Get inverse of -dl^2(theta) */
	double invDet = 1 / (dl2[0]*dl2[2] - dl2[1]*dl2[1]);
	 
	/* Update theta */
    if (back == 0) {
        //backtracking
        update[0] = theta[0] + (invDet*dl2[2]*dl1[0] - invDet*dl2[1]*dl1[1])/2;
        update[1] = theta[1] + (invDet*dl2[0]*dl1[1] - invDet*dl2[1]*dl1[0])/2;
    } else {
        update[0] = theta[0] + (invDet*dl2[2]*dl1[0] - invDet*dl2[1]*dl1[1]);
        update[1] = theta[1] + (invDet*dl2[0]*dl1[1] - invDet*dl2[1]*dl1[0]);
    }
	return(update);
}





