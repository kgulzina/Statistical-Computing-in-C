#include <stdio.h>

int main(void)
{
	float f = 0.3333333;
	float g = 0.13;
	int *p2 = (int*) &g;
	int *p = (int*) &f;
	*p = (*p2 >> 1);
	printf("%.30f\n", f);
	if (f == 0) {
	printf("nope");}
	return 0;
}