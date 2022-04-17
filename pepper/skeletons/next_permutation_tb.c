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
	int k, cor, ci;

	// Verify that i and j are correct
	// 0 <= i < j < n
	assert_zero(0 > i);
	assert_zero(i >= j);
	assert_zero(j >= n);
	// C_i < C_i+1 && forall k > i, C_k > C_k+1 && c_j > c_i && c_j+1 <= c_i
	for (k = 0; k < MAX_N - 1; k++) {
		if (k < n - 1) {
			// if k != j, cor is c[k+1], otherwise cor is c[i]
			if (k == j) cor = ci;
			else cor = input->c[k+1];
			
			if (k == i) {
				ci = input->c[k];
				assert_zero(input->c[k] >= input->c[k + 1]);
			// if k != j, cor is c[k+1], otherwise cor is c[i]
			} else if (k > i) {
				assert_zero(input->c[k] <= cor);
				assert_zero(cor < input->c[k+1]);
			}
		}
	}

	// if k < i, d[k] = c[k]
	// if k == i, d[k] = c[j]
	// if k == j, d[k] = c[n - i + 1]
	// otherwise, d[k] = c[n - k + 1]
	for (k = 0; k < MAX_N; k++) {
		if (k < n) {
			if (k < i) cor = k;
			else if (k == i) cor = j;
			else if (k == j) cor = n - i + 1;
			else cor = n - k + 1;
			assert_zero(input->c[k] != output->d[cor]);
		}
	}	
}
