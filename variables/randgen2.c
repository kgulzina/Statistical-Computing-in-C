/* program to illustrate the use of character arrays */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) 
{
	char ch[6] = {'h','e','l','l','o',0};
	putchar(ch[0]);
	putchar(ch[1]);
	putchar(ch[2]);
	putchar(ch[3]);
	putchar(ch[4]);
	putchar('?');
	putchar('\n');		/* preceding outputs: hello? */
	puts(ch);		/* outputs: hello */
	strcpy(ch, "bye");
	puts(ch);		/* outputs: bye */
	ch[3] = '!';
	puts(ch);		/* outputs: bye!o */	
	strcpy(ch, "bye");	/* try again */
	strcat(ch, "!");
	puts(ch);		/* outputs: bye! */
	return EXIT_SUCCESS;
}