#include <stdio.h>

int main(void)
{
	char str[5] = "hello";
	printf("%s\n", str);
	// str = "h3llo"; // you cannot use = to initialize array
	strcpy(str, "h3llo"); // takes two arrays, catches the missing space,
	// and overwrites that space. Takes that space randomly from the system. Might be the space of desktop.
	puts(str);
}