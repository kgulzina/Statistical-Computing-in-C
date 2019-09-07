/* STAT580/Homework2/Gulzina Kuttubekova/kgulzina@iastate.edu
 * Chapter3/Problem3/Another Random Number Generator
 */

/* Part 1: We have to write C implementation of the pseudocode to generate
 * single uniform random number and print it to the screen with 15 significant
 * digits, followed by a newline. 
 * Output when we start with seed = {45478,9487383} is 0.448998555564788   
 */


/* Part 2: We will choose our own seeds in the new version below:
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
	unsigned int ui_x, ui_y;	/* initial seeds for the algorithm*/	
	double unidev;			/* will hold result; not initialized*/
	
	ui_x = 80808;			/* initialize to any numbers*/
	ui_y = 13131313;	

	ui_x = 36969 * (ui_x & 65535) + (ui_x >> 16);	/* update initial values*/	
	ui_y = 18000 * (ui_y & 65535) + (ui_y >> 16);
	
	/* promote to double by casting the denominator */
	unidev = ((ui_x << 16) + ui_y + 1) / (double) (pow(2,32) + 2);

	printf("%.15f\n", unidev);

	return EXIT_SUCCESS;
}

/* Comments: After changing the initial values = seeds, we observed totally
 * different uniform "random" number.    
 */