#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct foo {
	int x;
	int y;
};

int main() {
	struct foo x;

	struct foo *y = malloc(sizeof(struct foo));

	memset (y, 0, sizeof(*y));

	struct foo *z = malloc(sizeof(x));

	memset (z, 0, sizeof(*z));

	struct foo *a = malloc(sizeof(x) * 5);

	memset (a, 0, sizeof(z) * 5);

	struct foo *b = malloc(0x41);

	memset (b, 0, 0x41);

	free (b);
	free (a);
	free (z);
	free (y);

	return 0;
}
