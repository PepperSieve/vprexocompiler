#include <stdint.h>

struct In {
	uint32_t n;
	uint32_t c[MAX_N];
	// D list, i and j provided by the prover
	// c_j provided by the prover. Surprisingly, we do not need c_i.
	uint32_t d[MAX_N];
	uint32_t i;
	uint32_t j;
	uint32_t ci, cip1, cj, cjp1;
};

struct Out {
	uint32_t d[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int i = input->i;
	int j = input->j;
	int n = input->n;
	int k, cor;
	int ci = input->ci;
	int cj = input->cj;
	int cip1 = input->cip1;
	int cjp1 = input->cjp1;
	uint32_t cc[MAX_N];

	// Copy C to CC and modify CC
	for (k = 0; k < MAX_N; k++) {
		if (k < n) cc[k] = input->c[k];
	}

	// Verify that i and j are correct
	// 0 <= i < j < n
	assert_zero(0 > i);
	assert_zero(i >= j);
	assert_zero(j >= n);

	// c[i] < c[i+1] && c[j] > c[i] && c[j+1] < c[i]
	assert_zero(ci >= cip1);
	assert_zero(cj <= ci);
	if (j != n - 1) assert_zero(cjp1 >= ci);

	// Swap c[i] and c[j]
	for (k = 1; k < MAX_N; k++) {
		if (k == i) {
			assert_zero(cc[k] - ci);
			cc[k] = cj;
		}
		if (k == i + 1) assert_zero(cc[k] - cip1);
		if (k == j) {
			assert_zero(cc[k] - cj);
			cc[k] = ci;
		}
		if (k == j + 1) assert_zero(cc[k] - cjp1);
	}

	// if k < i, d[k] = c[k]
	// otherwise, d[k] = c[n - k + 1]
	for (k = 0; k < MAX_N; k++) {
		if (k < n) {
			if (k <= i) cor = k;
			else cor = n - k + i;
			assert_zero(input->d[k] != cc[cor]);

			// forall k > i, d[k] < d[k+1]
			if (k != 0 && k - 1 > i)
				assert_zero(input->d[k-1] >= input->d[k]);
		}
	}	
}
