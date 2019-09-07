/**
 * @file em.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION: ksjhfaifbsis
 *
 * Compile as:
 * gcc -0 em em.c -Wall -pedantic -lm
 *
 * Usage:
 *	./em
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M 600
#define K 2			//idk the real value ??? Sepideh, Soura?
#define N 10
#define SKIPLINES 3

/* Declare functions */
float mvnorm(float mu[], float s[][N], float x[]);
double **estep(float mu[][N],  float s[][N][N], float p[], float x[][N], int K, int M, int N);
float determinantOfMatrix(float mat[N][N], int n);
void getCofactor(float mat[N][N], float temp[N][N], int p, int q, int n);
void inv(float A[N][N], float **I);


int main(void)
{

	/* Read initial values from data file -- Sepideh for you :) */
    double mu[K][N];
    double p[K];
    double s[K][N][N];

    // some code for that		            
    
 
    
  
    /* Read data from file */
    FILE *fp;								/* FILE object declaration */

	fp = fopen("stat580_hw4-1.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't open the file!\n");
		return EXIT_FAILURE;
	}

    /* allocate memory for input.txt */
	char line[2000];
	double x[600][10];						/* Gets the real data */
	int id[600];
	char class[2];

	/* Read in and discard first three lines */
	for (int i = 0; i < SKIPLINES; i++) fgets(line, sizeof(line), fp);
	
	for (int row = 0; row < M; row++)
	{
		for (int col = 0; col < N; col++)
		{
			if (col == 10) {
				fscanf(fp, "%s", class);
				if(!strcmp(class, "NA")) {
					id[row] = 0;
				} else {
					id[row] = atoi(class);
				}
			} else {
				fscanf(fp, "%lf", &x[row][col]);
			}
		}
	}
	fclose(fp); 


	//Here comes the the EM algorithm;
	/* E part is already written by Soura and it outputs [M][K] dimensional array with posterior probabilities. I will use that here to obtain new estimates */
	double **pi = malloc(M * sizeof(double));
	for (int i = 0; i < M; i++) {
		pi[i] = malloc(K * sizeof(double));
	}

	do {
		/* E step */
		pi = estep(mu, s, p, x, K, M, N);

		/* M step */
		double temp1 = 0, temp2 = 0, temp3 = 0;
		for (int k = 0; k < K; k++) {
			/* update pi_k */
			for (int i = 0; i < M; i++) {
				temp1 += pi[i][k];
			}
			p[k] = temp1 / M;

			/* update mu_k */
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < M; i++) {
					temp2 += pi[i][k] * x[i][j];
				}
				mu[k][j] = temp2 / temp1;
			}

			/* update sigma_k */
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					for (int m = 0; m < M; m++) {
						temp3 += (x[m][i]-mu[k][i])*(x[m][j]-mu[k][j]);
					}
					s[k][i][j] = temp3 / temp1;
				}
			}	
		} while (/* here is the convergence criteria: Sepideh ??? idk */);

    return(EXIT_SUCCESS);
}



/***********************************************************************/
/********************** Supplementary functions ************************/
/***********************************************************************/
/* Function to compute E step */
double **estep(float mu[][N],  float s[][][N], float p[], float x[][N], int K, int M, int N)
{
	double **pi = malloc(M * sizeof(double));
	for (int i = 0; i < M; i++) {
		pi[i] = malloc(K * sizeof(double));
	}
	int i, j, k, m;
	double temp1[N][N], temp2[N], temp3[N], den[K], sden;
	
	for(m = 0; m < M; m++){
		for(k = 0; k < K; k++){
			for(i = 0; i < N; i++){
				for(j = 0; j < N; j++){
					temp1[i][j] = s[k][i][j];
				}
			}
			for(i = 0; i < N; i++){
				temp2[i] = mu[k][i];
			}
			for(j = 0; j < N; j++){
				temp3[j] = x[m][j];
			}
			den[k] = p[k]*mvnorm(temp2, temp1, temp3);
			}	
		}
		sden = 0;
		for(k = 0; k < K; k++){
			sden = sden + den[k];
		}
		for(k = 0; k < K; k++){
			pi[m][k] = den[k]/sden;
		}
	}
	return(pi);
}



/* Function to calculate the pdf of a p dimensional multivariate normal distribution */
float mvnorm(float mu[], float s[][N], float x[])
{
	float prob = 0.0, qval = 0.0;	
	float det = determinantOfMatrix(s, N);	
	float **I;
	float diff[N];
	float pi = 3.14;
	int i, j;
	I=(float **)malloc(N*sizeof(float *));			
	for(i=0;i<N;i++)
		I[i]=(float *)malloc(N*sizeof(float));

	inv(s, I);

	/* Calculating difference between the data point and mean */
	for(i = 0; i < N; i++){
		diff[i] = x[i] - mu[i];
	}

	/* Calculating the quadratic form on the exponent */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			qval = qval + I[i][j]*diff[i]*diff[j];
		}	
	}

	/* Calculating the pdf */
	prob = ((1/(pow((2*pi), N/2)))*(1/pow(det, 0.5)))*exp(-(0.5*qval));

	return prob;	
}



/* Function to get cofactor of mat[p][q] in temp[][]. n is current 
dimension of mat[][] */
void getCofactor(float mat[N][N], float temp[N][N], int p, int q, int n) 
{ 
    int i = 0, j = 0; 
  
    // Looping for each element of the matrix 
    for (int row = 0; row < n; row++) 
    { 
        for (int col = 0; col < n; col++) 
        { 
            //  Copying into temporary matrix only those element 
            //  which are not in given row and column 
            if (row != p && col != q) 
            { 
                temp[i][j++] = mat[row][col]; 
  
                // Row is filled, so increase row index and 
                // reset col index 
                if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
}




/* Function to find the inverse of a functions */
void inv(float A[N][N], float **I)
{
	float temp;
	int i,j,k,matsize;

	matsize = N;



	for(i=0;i<matsize;i++)									//automatically initialize the unit matrix, e.g.
		for(j=0;j<matsize;j++)								//	-       -
			if(i==j)										// | 1  0  0 |
				I[i][j]=1;									// | 0  1  0 |
			else											// | 0  0  1 |
				I[i][j]=0;									//  -       -
/*---------------LoGiC starts here------------------*/		//procedure to make the matrix A to unit matrix
	for(k=0;k<matsize;k++)									//by some row operations,and the same row operations of
	{														//Unit mat. I gives the inverse of matrix A
		temp=A[k][k];										//'temp' stores the A[k][k] value so that  A[k][k] will not change
		for(j=0;j<matsize;j++)								//during the operation A[i][j]/=A[k][k] when i=j=k
		{
			A[k][j]/=temp;									//it performs the following row operations to make A to unit matrix
			I[k][j]/=temp;									//R0=R0/A[0][0],similarly for I also R0=R0/A[0][0]
		}													//R1=R1-R0*A[1][0] similarly for I
		for(i=0;i<matsize;i++)								//R2=R2-R0*A[2][0]		,,
		{
			temp=A[i][k];									//R1=R1/A[1][1]
			for(j=0;j<matsize;j++)							//R0=R0-R1*A[0][1]
			{												//R2=R2-R1*A[2][1]
				if(i==k)
				break;									//R2=R2/A[2][2]
				A[i][j]-=A[k][j]*temp;						//R0=R0-R2*A[0][2]
				I[i][j]-=I[k][j]*temp;						//R1=R1-R2*A[1][2]
			}
		}
	}
}