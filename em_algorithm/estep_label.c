#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 3
#define K 2
#define M 2

// Function to get cofactor of mat[p][q] in temp[][]. n is current 
void getCofactor(float mat[N][N], float temp[N][N], int p, int q, int n) 
{ 
// dimension of mat[][] 
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

/* Recursive function for finding determinant of matrix. 
   n is current dimension of mat[][]. */
float determinantOfMatrix(float mat[N][N], int n) 
{ 
    float D = 0.0; // Initialize result 
  
    //  Base case : if matrix contains single element 
    if (n == 1){ 
	printf("mat[0][0]: %f\n", mat[0][0]);
        return mat[0][0]; 
    }
  
    float temp[N][N]; // To store cofactors 
  
    float sign = 1.0;  // To store sign multiplier 
  
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

//Function to calculate the pdf of a p dimensional multivariate normal distribution

float mvnorm(float mu[N], float s[N][N], float x[N]){

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

	//Calculating difference between the data point and mean

	for(i = 0; i < N; i++){
		diff[i] = x[i] - mu[i];
	}

	//Calculating the quadratic form on the exponent

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			qval = qval + I[i][j]*diff[i]*diff[j];
		}	
	}

	//Calculating the pdf

	prob = ((1/(pow((2*pi), N/2)))*(1/pow(det, 0.5)))*exp(-(0.5*qval));

	return prob;


	
}

// the E step of the algorithm. Takes input the data, the initial values of the parameters and outputs a M*N matrix of posterior probabilities. The output matrix is pi.
// K is the number of groups, N the dimension (11 here) and M is the number of data points.

void estep(float mu[K][N], , float s[K][N][N], float p[K], float x[M][N]){

	float pi[M][K];
	int i, j, k, m;
	float temp1[N][N], temp2[N], temp3[N], den[K], sden;
	
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
}

//Take imput the matrix of posterior probabilities and return the data labels.
int label(float pi[M][K]){
	
	int lab[M], i, j;
	float temp[K];
	float max = 0;

	for(i = 0, i < M; i++){
		for (j = 0; j < K, j++){
			temp[j] = pi[i][j];
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
	return 0;
	
}

int main(int argc, char **argv){

	float A[3][3] = {{0.5, 0, 0},
			{0, 0.5, 0},
			{0, 0, 0.5}};

	int i, j;

	float det = determinantOfMatrix(A, N);

	printf("Determinant: %f\n", det);

	float **I;
	I=(float **)malloc(N*sizeof(float *));			//memory allocation forindentity matrix I(matsize X matsize)
	for(i=0;i<N;i++)
		I[i]=(float *)malloc(N*sizeof(float));

	inv(A, I);

	for(i=0; i < N; i++){
		for(j=0; j < N; j++){
			printf("%f ", I[i][j]);
		}
		printf("\n");
	}

	float mu[3] = {0, 0, 0};
	float x[3] = {0.9, 0.9, 0.3};
	
	float pr = mvnorm(mu, A, x);
	printf("Probability: %f ", pr);

		

	return 0;
}
