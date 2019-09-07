/* Just trial example */

/* How to generate categorical random variable */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	srand(12345);	/* takes unsigned integer */	
	double Pr[] = {0.36644, 0.1529688, 0.2162136, 0.2641735};
	double r = rand() / ((double) RAND_MAX +1);
	int X = 0;
	for (double dsum = Pr[X]; dsum < r; dsum += Pr[++X]);
	return 0;
}