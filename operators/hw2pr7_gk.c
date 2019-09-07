/* STAT580/Homework2/Gulzina Kuttubekova/kgulzina@iastate.edu
 * Chapter3/Problem7
 */

/* Part 1: We have to write a program which takes in any float or integer
 * and prints the binary representation of it.  
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{	
	if (!strcmp(argv[1], "-i")) {
		int a;
		printf("Enter a number: ");
		scanf("%d", &a);		/* takes in integer */
		
		int i, count = 1;
		
		for (i = (1 << 31); count < 33 ; a = a << 1, count++) {
			(a & i) ? printf("1") : printf("0");
			if (count % 8 == 0) {
				printf(" ");
			}
		}
	}		
	if (!strcmp(argv[1], "-f")) {
		float f;			/* takes in float */
		printf("Enter a number: ");
		scanf("%f", &f);
		
		int *j = (int*) &f;  
		int i, count = 1;
		
		for (i = (1 << 31); count < 33; *j = *j << 1, count++) {
			(*j & i) ? printf("1") : printf("0");
			if (count % 8 == 0) {
				printf(" ");
			}
		}
	}

	return EXIT_SUCCESS;
}











