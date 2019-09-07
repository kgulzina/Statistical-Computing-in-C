/* STAT580/Homework2/Gulzina Kuttubekova/kgulzina@iastate.edu 
 * Chapter3/Problem9
 */

/* Part 1: We have to */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

int main(int argc, char *argv[])
{			
	if (!strcmp(argv[1], "-f")) {
		float f;							/* takes in float */
		printf("Enter a number: ");
		scanf("%f", &f);
		
		/* to ensure program takes in only normal float */
		if (f < FLT_MIN) {
			printf("You entered subnormal float, please enter normal");
			return EXIT_FAILURE;
		}
		
		float exp_zero_maker = -1.1754942E-38;
		float new_value = 0;
		int *adr1 = (int*) &f;						    /* takes the memory adress of entered float */
		int *adr2 = (int*) &exp_zero_maker; 				/* takes the memory adress of exp_zero_maker float */
		int *adr3 = (int*) &new_value;
		
		/* to make the exponent part of float binary representation all zero and to
		add one bit to the significand part of entered float bit representation */
		*adr3 = (*adr1 & *adr2) + 1;

		/* to calculate the relative difference between new value and entered value */
		printf("%f\n", new_value - f); 	//see the comments
	} else {
		printf("Please enter float number");
		EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


/* Comments: I assume that the manipulation of the adress of float new_value, will change its value,
 * however, it did not work. If it works, my code should have been running
 * smoothly. I took help from TA and did everything as he suggested. */





