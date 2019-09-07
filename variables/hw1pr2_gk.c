/* STAT580/Homework1/Gulzina Kuttubekova/kgulzina@iastate.edu
 * Chapter2/Problem2
 */

/* Part 1: We have to rewrite AS 183 code, using unsigned char for the data 
 * type of the seeds. */


/* Simple random number generator. Tis program generates one Uniform(0,1)
 * pseudo-random deviate based on Wichmann and Hill's AS 183.
 */

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	unsigned char ix, iy, iz;	/*< initial seeds for the algorithm */
	double unidev;			/*< will hold result; not initialized */

	ix = 23456;			
	iy = 13454;			
	iz = 8992;	

	/* part 1 */
	//ix = (171 * ix) % 30269;
	//iy = (172 * iy) % 30307;	
	//iz = (170 * iz) % 30323;

	//unidev = ix / 30269.0 + iy / 30307.0 + iz / 30323.0;

	/* part 2*/
	unidev = ((171*ix)%30269) / 30269.0 + ((172*iy)%30307) / 30307.0 + ((170*iz)%30323) / 30323.0;

	unidev -= (int) unidev;		/*check unidev is representable */

	printf("%f\n", unidev);

	return EXIT_SUCCESS;
}

/* Comments:

 * By compiling this code, we observe errors, such as: 
 * unsigned conversion from int to unsigned char makes ix, which is declared
 * as 24356, 160. Same for iy, iz: iy from 13454 becomes 142 and iz from 
 * 8992 becomes 32. This is the warning only, and the code still works. 
 * The only warning comes in the longer run, if we try to use this algorithm
 * multiple times, say more than 256 times. Since unsigned chars are mapped
 * to integers from 0 to 255 at maximum, range of possible output "random"
 * numbers is smaller than if we were using the original assumption: ix, iy,
 * iz are integers. */




/* Part 2: Now we have to rewrite the code, by replacing 4 lines with a single
 * line by using parantheses to group and prioritize operations.  */


/* Comments:

 * After changing the lines, i.e writing the four lines into one we observe
 * the same warnings with unsigned conversion. However, the code still works
 * but it returns different "random" number  = 0.889183 compared to the 
 * previous case = 0.012942. It means that by using parantheses, i.e by
 * grouping and prioritizing the operations, the final outcome has changed. 
 * From Chapter 3, we know that C does not actually impose an order of 
 * evaluation. There is no requirement that function is evaluated from left
 * to right. Hence before restricting the order and using the parantheses, 
 * code was generating totally different number. */ 






