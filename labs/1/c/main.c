#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define LEVELS 5
#define N_NODES ((1 << LEVELS) - 1)

struct node {
	unsigned int marked : 1;
	unsigned int done   : 1;
};

void swap(unsigned int *a, unsigned int *b) {
	unsigned int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void shuffle(unsigned int *array) {
	size_t roll;
	for (unsigned int i = N_NODES; i > 2; --i) {
		roll = (size_t) (random() % i);
		swap(&array[i - 1], &array[roll]);
	}
}

void print_tree(struct node *tree) {
	printf("PRINTING TREE:\n");
	for (unsigned int i = 0; i < LEVELS; ++i) {
		unsigned int n = 1 << i;
		for (unsigned int j = (1 << i) - 1; j < (1 << (i + 1)) - 1; ++j) {
			printf("%d:%c%c  ", j,
			       tree[j].marked ? 'm' : ' ',
			       tree[j].done   ? 'd' : ' ');
		}
		printf("\n");
	}
	printf("TREE OVER\n");
}

size_t l_child(size_t parent) {
	return 2 * parent + 1;
}

size_t r_child(size_t parent) {
	return 2 * parent + 2;
}

size_t parent(size_t child) {
	return (child - 1) / 2;
}

size_t sibling(size_t idx) {
	return idx % 2 ? idx + 1 : idx - 1;
}

int leaf_p(size_t idx) {
	return idx >= N_NODES / 2;
}

void mark(struct node *tree, size_t idx) {
	struct node *u = &tree[idx];
	u->marked = 1;

	if (leaf_p(idx) ||
	    (tree[l_child(idx)].done && tree[r_child(idx)].done)) {
		u->done = 1;
	}

	if (idx != 0 && tree[sibling(idx)].marked) {
		mark(tree, parent(idx));
	} else if (idx != 0 && tree[parent(idx)].marked) {
		mark(tree, sibling(idx));
	}

	if (leaf_p(idx))
		return;

	if (tree[l_child(idx)].marked && !tree[r_child(idx)].marked) {
		mark(tree, r_child(idx));
	} else if (!tree[l_child(idx)].marked && tree[r_child(idx)].marked) {
		mark(tree, l_child(idx));
	}
}

int main() {
	struct node bob[N_NODES];
	unsigned int alice[N_NODES];

	srandom((unsigned int)time(NULL));

	struct node empty = {};

	for (int i = 0; i < N_NODES; ++i) {
		bob[i] = empty;
		alice[i] = i;
	}

	shuffle(alice);

	int i;
	for (i = 0; i < N_NODES; ++i) {
		printf("Marking %d!\n", alice[i]);
		mark(bob, alice[i]);

		print_tree(bob);
		if (bob[0].done) {
			break;
		}
	}

	printf("Finished after %d rounds\n", i);

	return 0;
}
