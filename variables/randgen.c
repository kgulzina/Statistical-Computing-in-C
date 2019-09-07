/**
 * Simple random number generator. Tis program generates one Uniform(0,1)
 * pseudo-random deviate based on Wichmann and Hill's AS 183.
 */

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int ix, iy, iz;	/*< initial seeds for the algorithm */
	double unidev;	/*< will hold result; not initialized */

	ix = 23456;	/* initialize to any numbers */
	iy = 13454;	/* between 1 and 30,000 */
	iz = 8992;	

	ix = (171 * ix) % 30269;	/* assumes integer arithmetic */
	iy = (172 * iy) % 30307;	/* up to 5,212,632 */
	iz = (170 * iz) % 30323;

	/* promote to floating-point arithmetic by using real constants */
	unidev = ix / 30269.0 + iy / 30307.0 + iz / 30323.0;

	unidev -= (int) unidev;		/*check unidev is representable */

	printf("%f\n", unidev);

	return EXIT_SUCCESS;
}
