

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	float a;
	int temp;
	char buffer[32];

	printf("Enter a number: ");
	scanf("%f", &a);

	itoa(a, buffer, 2);

	printf("%s", buffer);
}