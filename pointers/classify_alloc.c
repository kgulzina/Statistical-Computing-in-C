/**
 * @file classify.c
 * @authors SSGL
 * @version 0.1
 * @details
 *
 * Reads in the text file and returns text file with filled NA's
 *
 *
 * Compile as:
 *     gcc -Wall -pedantic -o classify classify.c -lm
 *
 * Run as:
 *     ./classify stat580_hw4-1.txt
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define SKIPLINES (1)
#define ncol 10

/* declarations of functions to be used */
double mean(double v[], int n);
double norm(double v[], double **invSn);
int classify(double v[], double d[][11], int id[]);

int main(void)
{
	FILE *fp, *fout;								/* FILE object declaration */

	fp = fopen("stat580_hw4-1.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't open the file!\n");
		return EXIT_FAILURE;
	}

	/* allocate memory for input.txt */
	char line[2000];  						/* Make the assumtion that a line < 255 chars */
	double d[600][11];
	int id[600];
	char class[2];

	/* Read in and discard first three lines */
	for (int i = 0; i < SKIPLINES; i++) fgets(line, sizeof(line), fp);
	
	for (int row = 0; row < 600; row++)
	{
		for (int col = 0; col < 11; col++)
		{
			if (col == 10) {
				fscanf(fp, "%s", class);
				if(!strcmp(class, "NA")) {
					id[row] = 0;
				} else {
					id[row] = atoi(class);
				}
			} else {
				fscanf(fp, "%lf", &d[row][col]);
			}
		}
	}
	fclose(fp);

	/* classify each x with id == 0, leave rest the same */
	double temp[10];
	for (int row = 0; row < 600; row++) {
		if (id[row] == 0) {
			for (int j = 0; j < 11; j++) {
				temp[j] = d[row][j];
			}
			d[row][10] = classify(temp, d, id);
		} else {
			d[row][10] = (double) id[row];	/* Leave as it is */
		}
	}

	/* write the result to the new output.txt file */
	fout = fopen("output.txt", "w");
	for (int i = 0; i < 600; i++) {
		for (int j = 0; j < 11; j++) {
			if (j == 10) {
                fprintf(fout, "%lf\n", d[i][j]);    /* close the line */
            } else {
                fprintf(fout, "%lf ", d[i][j]);
            }
		}
	}
	fclose(fout);

	return(EXIT_SUCCESS);						
}
    



/***************************************************************************/
/************************* Supplementary functions *************************/
/***************************************************************************/

int classify(double v[], double d[][11], int id[]) {
/* Takes in a vector: x of dimension 10, observed data, and returns class of x.
 */
	/* take each x which is unclassified */
	double min = 585.0;						/* to store the minimum norm */
	int res;								/* to store resulting class */

	for (int class = 1; class < 6; class++) {
		/* we have to learn the sample size: n*/
		int n = 0;
		for (int i = 0; i < 600; i++) {
			if (id[i] == class) {
				n++;
			} else { 
				continue;
			}
		}
		printf("%d\n", n);

		/* allocate memory for the target matrix, where nrow = n */
		double **matrix = malloc(n * sizeof(double *));
		for (int i = 0; i < n; i++) {
			matrix[i] = malloc(ncol * sizeof(double));
		}

		/* store target matrix: observed x's of class: "k" */
		int count = 0;

		for (int i = 0; i < 600; i++) {
			if (id[i] == class) {
				for (int j = 0; j < 10; j++) {
					matrix[count][j] = d[i][j];
				}
				count++;
			} else {
				continue;
			}
		}

		/* get the target vector: (x - m_k) */
		double tv[10];
		double mean_v[10];

		for (int j = 0; j < 10; j++) {
			/* select the k'th variable */
			double temp[n];

			for (int i = 0; i < n; i++) {
				temp[i] = matrix[i][j];
			}
			mean_v[j] = mean(temp, n);			/* required for covarince !!!  */
			tv[j] = v[j] - mean_v[j];
		}

		/* get the inverse covariance of observed x's of class: "k" */
		/* (!) to optimize memory usage, we will store cov and invSn in the same adress */
		double **invSn = malloc(ncol * sizeof(double *));
		for (int i = 0; i < ncol; i++) {
			invSn[i] = malloc(ncol * sizeof(double));
		}

		/* calculate the covariance here */
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				double temp = 0;
				if (i == j) {
					for (int s = 0; s < n; s++) {
						temp += pow((matrix[s][i] - mean_v[i]), 2);
					}
					invSn[i][j] = temp / (n-1);
				} else {
					for (int s = 0; s < n; s++) {
						temp += (matrix[s][i] - mean_v[i])*(matrix[s][j] - mean_v[j]);
					}
					invSn[i][j] = temp / (n-1);
				}
			}
		}
		free(matrix);

		/* Calculate the inverse of covariance */
		FILE *fp, *fout;

		/* write cov matrix into .txt file, record by row, space tab */
		fout = fopen("cov.txt", "w");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if ( i == 9 && j == 9) {
					fprintf(fout, "%lf\n", invSn[i][j]);	/* close the line */
				} else {
					fprintf(fout, "%lf ", invSn[i][j]);
				}
			}
		}
		fclose(fout);
		//free(invSn);

		/* command to shell */
		system("Rscript get_inverse.R");

		fp = fopen("inverse.txt", "r");
		if (fp == NULL) {
			fprintf(stderr, "Couldn't open the file!\n");
			return EXIT_FAILURE;
		}

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				fscanf(fp, "%lf", &invSn[i][j]);
			}
		}
		fclose(fp);

		/* get the norm of target vector: (x - m_k) */
		double norm_v;
		norm_v = norm(tv, invSn);

		/* compare with the minimum norm */
		if (norm_v < min) {
			min = norm_v;
			res = class;
		} else {
			continue;
		}
	}
	return(res);
}




double mean(double v[], n) {
/* Takes in a vector of values and its length. Returns the mean of a vector.
 */
	double mean_v;
	double temp = 0;
	int i = 0;

    for (int i = 0; i < n; i++) {
        temp += v[i];
    }
	mean_v = temp / (double) n;
	return(mean_v);
}



double norm(double v[], double **invSn) {
/* It takes in a vector of dimension 10, inverse of sample covariance matrix
 * of the cluster: j. Calculates and returns the norm of a vector.
 */
	double norm_v;							/* to store the output */
	double part1[10];					
	double part2 = 0;						/* to store the inner part */	
	double temp;

	/* multiply the first part */
	for (int i = 0; i < 10; i++) {
		temp = 0;
		for (int j = 0; j < 10; j++) {
			temp += v[j] * invSn[i][j]; 
		}
		part1[i] = temp;
	}

	/* multiply the second - inner part */
	for (int i = 0; i < 10; i++) {
		part2 += part1[i] * v[i];
	}
	norm_v = sqrt(abs(part2));
	return(norm_v);
}


















