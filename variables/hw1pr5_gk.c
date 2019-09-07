/* STAT580/Homework1/Gulzina Kuttubekova/kgulzina@iastate.edu
 * Chapter2/Problem5 
 */


/* Part1: We have to convert unsigned (0011101101010111)_2 to hexadecimal
 * format by hand. It is given in the attached pdf file.  
 */


/* Part2: Initialize a unsigned short const with the hexedecimal value, and
 * print out the result as an integer */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	unsigned short const num = 0x3B57 ;
	printf("final result is %d\n", (int) num);
	return EXIT_SUCCESS;
}