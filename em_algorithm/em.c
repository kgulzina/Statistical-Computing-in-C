/**
 * @file em.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * DESCRIPTION: This code run EM algorithm derived on Gaussian misture model. * We have derived algorithm and written it separately in pdf file.
 * This code reads in initial values generated in R and prints out .txt file
 * with labels for n = 600 seqeunces. 
 * Our code has segmentation fault, and we did not have enough time to fix it 
 * by deadline, but we will try to fix it anyway after the deadline too.
 *
 * Compile as:
 * gcc -o em em.c -Wall -pedantic -lm
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
#define K 6		
#define N 10
#define SKIPLINES 3

/* Declare functions */
double mvnorm(double mu[], double s[][N], double x[]);
double **estep(double mu[][N], double s[][N][N], double p[], double x[][N], int kk, int mm, int nn);
double determinantOfMatrix(double mat[][N], int n);
void getCofactor(double mat[][N], double temp[][N], int p, int q, int n);
void inv(double A[][N], double **I);


int main(void)
{

	/* Initial Values coded in R-- See the R code.*/
	double p_prev[K] = {0.1666667, 0.1666667, 0.1666667, 0.1666667, 0.1666667, 0.1666667};
	double p_next[K];
    
	FILE *fpmu;	
	double mu_prev[K][N];
	double mu_next[K][N];	

	fpmu = fopen("mu_rep.txt", "r");
	for(int i = 0; i < K; i++)
	{
		for(int j = 0; j < N; j++)
		{
			fscanf(fpmu, "%lf", &mu_prev[i][j]);
		}
	}
	fclose(fpmu);    


	FILE *fps;	
	double s_prev[K][N][N];
	double s_next[K][N][N];	

	fps = fopen("s_rep.txt", "r");
	for(int k = 0; k < K; k++)
	{
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
				fscanf(fps, "%lf", &s_prev[k][i][j]);
		}
	}
	fclose(fps);        
    
 
    /* Read data from file */
    FILE *fp;								/* FILE object declaration */

	fp = fopen("stat580_hw4-1.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't open the file!\n");
		return EXIT_FAILURE;
	}

    /* allocate memory for input.txt */
	char line[6000];
	double x[600][10];						/* Gets the real data */
	int id[600];
	char class[6];

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


	/* Here comes the the EM algorithm */
	double **pi = malloc(M * sizeof(double));
	for (int i = 0; i < M; i++) {
		pi[i] = malloc(K * sizeof(double));
	}

	/* To alternate between two parameter sets */
	int count = 1;	
	double sum1 = 0.0, sum2 = 0.0;
	double norm1, norm2, euc_norm;
			 

	do {
		/* E step */
		if ((count & 1) == 1) {
			pi = estep(mu_prev, s_prev, p_prev, x, K, M, N);
		} else {
			pi = estep(mu_next, s_next, p_next, x, K, M, N);
		}
		
		/* M step */
		double temp1 = 0, temp2 = 0, temp3 = 0;
		for (int k = 0; k < K; k++) {
			/* update pi_k */
			for (int i = 0; i < M; i++) {
				temp1 += pi[i][k];
			}
			if ((count & 1) == 1) { //count = odd
				p_next[k] = temp1 / M;
			} else {
				p_prev[k] = temp1 / M;
			}

			/* update mu_k */
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < M; i++) {
					temp2 += pi[i][k] * x[i][j];
				}
				if ((count & 1) == 1) { //count = odd
					mu_next[k][j] = temp2 / M;
				} else {
					mu_prev[k][j] = temp2 / M;
				}
			}

			/* update sigma_k */
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					for (int m = 0; m < M; m++) {
						if ((count & 1) == 1) {
							temp3 += (x[m][i]-mu_prev[k][i])*(x[m][j]-mu_prev[k][j]);
						} else {
							temp3 += (x[m][i]-mu_next[k][i])*(x[m][j]-mu_next[k][j]);
						}
						
					}
					if ((count & 1) == 1) { //count = odd
						s_next[k][i][j] = temp3 / temp1;
					} else {
						s_prev[k][i][j] = temp3 / temp1;
					}
				}
			}
			count++;
		}
		for(int i = 0; i < M; i++) {
			for(int j = 0; j < N; j++){
				sum1 += (s_next[i][j] - s_prev[i][j]) * (s_next[i][j] - s_prev[i][j]);
			}
		}
		norm1 = sqrt((double)sum1);
		for (int i = 0; i < M; i++) {
			sum2 += (mu_next[i] - mu_prev[i]) * (mu_next[i] - mu_prev[i]);
		}
		norm2 = sqrt((double)sum2);
		euc_norm = norm1 + norm2;
	} while (euc_norm < (10^-5));

	/* here we assume we have  final results */
	int lab[M];
	double temp[K];
	double max = 0;

	for(int i = 0; i < M; i++){
		for (int j = 0; j < K; j++){
			temp[j] = pi[i][j];					//pi[][] with final estimates
		}
	max = temp[0];
		for(int k = 0; k < K; k++){
			if(max < temp[k]){
				max = temp[k];
				lab[i] = k;			
			}
		}
	}
	for(int l = 0; l < M; l++){
		printf("%d", lab[l]);	
	}

	/* write those labels into the txt file */
	FILE *fout;
	fout = fopen("labels.txt", "w+");
	if (fout == NULL) {
		fprintf(stderr, "Couldn't open the file!\n");
		return EXIT_FAILURE;
	}

	for (int l = 0; l < M; l++) {
		fprintf(fout, "%d", lab[l]);
	}
	fclose(fout);

    return(EXIT_SUCCESS);
}



/***********************************************************************/
/********************** Supplementary functions ************************/
/***********************************************************************/
/* Function to compute E step */
double **estep(double mu[][N], double s[][N][N], double p[], double x[][N], int kk, int mm, int nn)
{
	double **pi = malloc(mm * sizeof(double));
	for (int i = 0; i < mm; i++) {
		pi[i] = malloc(kk * sizeof(double));
	}
	int i, j, k, m;
	double temp1[nn][nn], temp2[nn], temp3[nn], den[kk], sden;
	
	for(m = 0; m < mm; m++){
		for(k = 0; k < kk; k++){
			for(i = 0; i < nn; i++){
				for(j = 0; j < nn; j++){
					temp1[i][j] = s[k][i][j];
				}
			}
			for(i = 0; i < nn; i++){
				temp2[i] = mu[k][i];
			}
			for(j = 0; j < nn; j++){
				temp3[j] = x[m][j];
			}
			den[k] = p[k]*mvnorm(temp2, temp1, temp3);	
		}
		sden = 0;
		for(k = 0; k < kk; k++){
			sden = sden + den[k];
		}
		for(k = 0; k < kk; k++){
			pi[m][k] = den[k]/sden;
		}
	}
	return(pi);
}



/* Function to calculate the pdf of a p dimensional multivariate normal distribution */
double mvnorm(double mu[], double s[][N], double x[])
{
	double prob = 0.0, qval = 0.0;	
	double det = determinantOfMatrix(s, N);	
	double **I;
	double diff[N];
	double pi = 3.14;
	int i, j;
	I=(double **)malloc(N*sizeof(double *));			
	for(i=0;i<N;i++)
		I[i]=(double *)malloc(N*sizeof(double));

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
void getCofactor(double mat[N][N], double temp[N][N], int p, int q, int n) 
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
void inv(double A[N][N], double **I)
{
	double temp;
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



/* Recursive function for finding determinant of matrix. 
   n is current dimension of mat[][]. */
double determinantOfMatrix(double mat[][N], int n) 
{ 
    double D = 0.0; // Initialize result 
  
    double temp[N][N]; // To store cofactors 
  
    double sign = 1.0;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int f = 0; f < n; f++) 
    { 
        // Getting Cofactor of mat[0][f] 
        getCofactor(mat, temp, 0, f, n);
        //printf("D: %f\n", D);	
        D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1); 
  
        // terms are to be added with alternate sign 
        sign = 0.0 - sign; 
    }

     
    printf("D: %f\n", D);	
  
    return D; 
}
