// We assume that the order within the same color does not matter
// Since the standard dutch flag is also does not respect the original order

#include <stdint.h>

struct item {
	// color: 0, 1, 2
	int color;
	int content;
};

struct In {
	uint32_t n;
	struct item* a[MAX_N];
	// b provided by the prover
	struct item* b[MAX_N];
	// k1: a[k1] RED && a[k1 + 1] WHITE
	// k2: a[k2] WHITE && a[k2 + 1] BLUE
	// Use -1 and n-1 if some of the colors does not exist
	uint32_t k1, k2;
	// Extra verification that every item in a corresponds to something in b
	uint32_t ord[MAX_N];
};

struct Out {
	struct item* b[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int i, ord;
	int n = input->n;
	int k1 = input->k1;
	int k2 = input->k2;
	struct item* tmp;
	// Use count to make sure everything is only used once
	int count[MAX_N];
	for (i = 0; i < MAX_N; i++) {
		count[i] = 0;
	}
	assert_zero(k1 < -1);
	assert_zero(k1 > k2);
	assert_zero(k2 >= n);
	for (i = 0; i < MAX_N; i++) {
		ord = input->ord[i];
		if (i < n) {
			assert_zero(ord < 0 || ord >= n);
			// Pigeonhole: if n ORD assigns n COUNT to 1, then every ORD
			// must have assigned a distinct COUNT
			count[ord] = 1;
			assert_zero(input->b[i] != input->a[ord]);
		} else count[i] = 1;
		tmp = input->b[i];
		if (i <= k1) assert_zero(tmp->color);
		else if (i <= k2) assert_zero(tmp->color - 1);
		else if (i < n) assert_zero(tmp->color - 2);
	}
	for (i = 0; i < MAX_N; i++) {
		assert_zero(count[i] - 1);
	}
}
