#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct foo {
	int x;
	int y;
};

struct bar {
	int x;
	int y;
};

int main() {
	struct foo x;

	struct foo *y = malloc(sizeof(struct foo));

	struct foo *z = malloc(sizeof(x));

	struct foo *a = malloc(5 * sizeof(x));

	struct bar *c = malloc(sizeof(x) * sizeof(struct bar));

	struct foo *d = malloc(sizeof(struct foo) * 5);

	struct foo *b = malloc(0x41);

	memset (y, 0, sizeof(*y));
	memset (z, 0, sizeof(*z));
	memset (a, 0, sizeof(z) * 5);
	memset (b, 0, 0x41);
	memset (d, 0, sizeof(struct foo) * 5);
	memset (c, 0, sizeof(*c));

	free (d);
	free (c);
	free (b);
	free (a);
	free (z);
	free (y);

	return 0;
}
