#include <stdint.h>

struct In {
	uint32_t n;
	uint32_t c[MAX_N];
	// D list, i and j provided by the prover
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
	// 0 <= i < j < n
	assert_zero(0 > i);
	assert_zero(i >= j);
	assert_zero(j >= n);
	// C_i < C_i+1 && forall k > i, C_k > C_k+1
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
	// C_j > C_i && C_j+1 < C_i
	int cj = c[j];
	assert_zero(cj <= ci);
	if (j + 1 < n) assert_zero(c[j + 1] >= ci);

	// Swap C_i and C_j
	c[i] = cj;
	c[j] = ci;

	// After swapping C_i and C_j,
	// verify D[0..i+1] = C[0..i+1]
	// and D[i+1..n] is the reverse of C[i+1..n]
	for (k = 0; k < MAX_N; k++) {
		if (k < n) {
			if (k <= i) assert_zero(input->d[k] != c[k]);
			else assert_zero(input->d[k] != c[n - k + i]);
		}
	}
}
