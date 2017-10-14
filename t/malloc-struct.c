#include <stdio.h>
#include <stdlib.h>

struct foo {
	int x;
	int y;
};

int main() {
	struct foo x;

	struct foo *y = malloc(sizeof(struct foo));

	struct foo *z = malloc(sizeof(x));

	struct foo *a = malloc(sizeof(x) * 5);

	return 0;
}
