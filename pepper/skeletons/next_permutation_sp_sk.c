#include <stdint.h>

struct In {
	uint32_t n;
	uint32_t c[MAX_N];
	// d list, i and j provided by the prover
	uint32_t d[MAX_N];
	uint32_t i;
	uint32_t j;
};

struct Out {
	uint32_t d[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int i = input->i;
	int j = input->j;
	int n = input->n;
	int k, ci;
	uint32_t c[MAX_N];
	for (k = 0; k < MAX_N; k++) {
		c[k] = input->c[k];
	}

	// Verify that i is correct
	// 0 <= i < j < n ==> i < n - 1
	assert_zero(i < 0);
	assert_zero(j <= i);
	assert_zero(j >= n);
	for (k = 0; k < MAX_N - 1; k++) {
		if (k < n - 1) {
			if (k == i) {
				ci = c[k];
				assert_zero(c[k] >= c[k + 1]);
			} else if (k > i) {
				assert_zero(c[k] <= c[k + 1]);
			}
		}
	}

	// Verify that j is correct
	int cj = c[j];
	assert_zero(cj <= ci);
	if (j + 1 < n) assert_zero(c[j + 1] >= ci);
	c[i] = cj;
	c[j] = ci;

	// Verify reverse
	for (k = 0; k < MAX_N; k++) {
		if (k < n) {
			if (k > i) assert_zero(input->d[k] != c[n - k + i]);
			else assert_zero(input->d[k] != c[k]);
		}
	}
}
