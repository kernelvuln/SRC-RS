#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *a = malloc(0x100 - 8);
	char *b = malloc(0x100 - 8);
	char *c = malloc(0x80 - 8);

	printf("---------------init---------------\n");
	printf("a: %p -> %p\n", a, a + 0x100 - 8);
	printf("b: %p -> %p\n", b, b + 0x100 - 8);
	printf("c: %p -> %p\n", c, c + 0x80 - 8);
	printf("----------------------------------\n");
	/* overflow */
	a[0x100 - 8] = 0x81;
	printf("overflow: %x\n", a[0x100 - 8]);

	free(b);
	free(c);

	b = malloc(0x100 + 0x80 - 8);

	printf("------------overflowed------------\n");
	printf("b: %p -> %p [*]\n", b, b + 0x100 + 0x80 - 8);
	printf("a: %p -> %p\nc: %p -> %p\n", a, a + 0x100 - 8, c, c + 0x80 - 8);
	printf("----------------------------------\n");
}
