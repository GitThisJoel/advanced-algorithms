#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

void shuffle(unsigned int *array, unsigned int length) {
	size_t roll;
	for (unsigned int i = length; i > 2; --i) {
		roll = (size_t) (random() % i);
		swap(&array[i - 1], &array[roll]);
	}
}

void print_tree(struct node *tree, unsigned int levels) {
	printf("PRINTING TREE:\n");
	for (unsigned int i = 0; i < levels; ++i) {
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

int leaf_p(size_t idx, unsigned int n_nodes) {
	return idx >= n_nodes / 2;
}

void mark(struct node *tree, unsigned int n_nodes, size_t idx) {
	struct node *u = &tree[idx];
	u->marked = 1;

	if (leaf_p(idx, n_nodes) ||
	    (tree[l_child(idx)].done && tree[r_child(idx)].done)) {
		u->done = 1;
	}

	if (idx != 0 && tree[sibling(idx)].marked) {
		mark(tree, n_nodes, parent(idx));
	} else if (idx != 0 && tree[parent(idx)].marked) {
		mark(tree, n_nodes, sibling(idx));
	}

	if (leaf_p(idx, n_nodes))
		return;

	if (tree[l_child(idx)].marked && !tree[r_child(idx)].marked) {
		mark(tree, n_nodes, r_child(idx));
	} else if (!tree[l_child(idx)].marked && tree[r_child(idx)].marked) {
		mark(tree, n_nodes, l_child(idx));
	}
}

unsigned int trial(unsigned int levels, unsigned int r) {
	unsigned int n_nodes = (1 << levels) - 1;
	struct node *bob = calloc(n_nodes, sizeof(struct node));
	unsigned int *alice = calloc(n_nodes, sizeof(unsigned int));

	for (unsigned int i = 0; i < n_nodes; ++i) {
		alice[i] = i;
	}

	shuffle(alice, n_nodes);

	unsigned int discount = 0;

	for (unsigned int i = 0; i < n_nodes; ++i) {
		switch(r) {
		case 1:
			mark(bob, n_nodes, random() % n_nodes);
			break;
		case 2:
		case 3:
			if (r == 3 && bob[alice[i]].marked) {
				++discount;
			} else {
				mark(bob, n_nodes, alice[i]);
			}

			break;
		default:
			printf("unknown random process\n");
			exit(1);
		}

		if (bob[0].done) {
			return i - discount;
		}
	}

}

int main() {
	srandom((unsigned int)time(NULL));

	printf("%d\n", trial(5, 3));

	return 0;
}
