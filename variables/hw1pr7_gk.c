/* STAT580/Homework1/Gulzina Kuttubekova/kgulzina@iastate.edu 
 * Chapter1/Problem7 
 */


/* Part1: The smallest magnitude float is 1.175494*10^{-38} in decimal. 
 * We can verify it by printing the floating point value of macro FLT_MIN, 
 * defined in float.h 
 */

/* Comments: 
 * It is true that the minimum float values in our computer is given as
 * 1.755494*^{-38} 
 */


#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int main(void)
{
	// part1: to verify the minimum float value
	printf("The minimum float values is %.6e\n", FLT_MIN);

	// part2
	float const a = FLT_MIN * 0.01;
	double const b = FLT_MIN * 0.01;

	printf("a = %.6e\n", a);
	printf("b = %.6e\n", b);

	return EXIT_SUCCESS;
}



/* Part2: How the number 'a', which is smaller than the minimum FLT_MIN, even 
 * representable as float? 
 */

/* Comment: 'a' is representable as float, since to store the floating point,
 * C needs sign, significand:S and exponent:E information in 32 bits.
 * Information such as sign, exponent and significand will be still stored, 
 * however, precision will not be retained, i.e there might be no room to 
 * store required exponent information. By required, I mean, true information. 
 */


/* Part3: Why does the float value not retain the "promised" six decimals of
 * precision, while the double value does? 
 */

/* Comment: The size of float is 32 bits and accuracy is 6 decimal places
 * of precision only, it cannot retain the "promised" six decimals
 * of precision, since the number 'a' has reached the possible minimum float 
 * representation. Hence 'a' is only an approximate float representation of 
 * FLT_MIN * 0.01. Other explanation can be found in the book: unnormalized
 * float smaller than the minimum normalized float is called subnormal number.
 * Subnormal numbers cannot be normalized since there is no room to store the
 * required Exponent value: E. Since some bits are wasted for the leading 
 * zero's, subnormal numbers in our case is encoded with the less precision. 
 *
 * The size of double is 64 bits and accuracy is 15 decimal places of 
 * precision. Hence it can "retain" the promised six decimals of precision, 
 * because there is a room for that precision, i.e there is a room for the 
 * required exponent:E value. 
 */