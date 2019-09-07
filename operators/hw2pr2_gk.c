/* STAT580/Homework2/Gulzina Kuttubekova/kgulzina@iastate.edu
 * Chapter3/Problem2 
 */

/* Part 1: We have to predict what will be printed by the code below
 * and provid our reasons.
 */

/* Comments: 
 * 1. We will evaluate operations inside if condition 
 * 2. Operations inside parantheses are evaluated next
 * 3. k++ has higher precedence, so value of k = 0 is returned first, then 
 * k is incremented by 1. 
 * 4. j && 0 || 1 >> evaluates to 1
 * 5. Boolean operations have higher precedence than comparison, so we check
 * if 1 == 1 >> returns 1
 * 6. Then i || 1 >>  returns 1, hence code prints Hello!
 * 7. Then code prints i = 0, j = 0. k = 0 and l = 1. k = 0 stays the same,
 * since k++ was not initalized to any constant. And k++ makes k = 1 only
 * in that specific line before it sees semicolon ;   
 */



/* Part 2: Write the code and compile it. Compare the results with the
 * predicted one.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i = 0, j = 0, k = 0, l = 1;
	if ( i || (j && k++ || l) == 1 )
		printf("Hello!\n");
	else
		printf("Bye!\n");
	printf("%d %d %d %d\n", i, j, k, l);

	return EXIT_SUCCESS;
}

/* Comments: code prints Hello! and 0 0 0 1, as we expected.
 */