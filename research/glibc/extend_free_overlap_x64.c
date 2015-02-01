#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *a, *b, *c;

	a = malloc(0x100 - 8);
	b = malloc(0x100 - 8);
	c = malloc(0x80 - 8);
	printf("---------------init---------------\n");
	printf("a: %p -> %p\nb: %p -> %p\n", a, a + 0x100 - 8, b, b + 0x100 - 8);
	printf("c: %p -> %p\n", c, c + 0x80 - 8);
	printf("----------------------------------\n");

	free(b);
	/*
	 * overflow into a
	 * the old chunk b's size becomes 0x181 instead of 0x101
	 * */
	a[0x100 - 8] = 0x81;
	printf("overflow: %x\n", a[0x100 - 8]);

	b = malloc(0x100 + 0x80 - 8);
	printf("------------overflowed------------\n");
	printf("b: %p -> %p [*]\n", b, b + 0x100 + 0x80 - 8);
	printf("a: %p -> %p\nc: %p -> %p\n", a, a + 0x100 - 8, c, c + 0x80 - 8);
	printf("----------------------------------\n");
}
