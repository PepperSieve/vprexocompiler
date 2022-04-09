#include <stdint.h>

struct In {
	uint32_t n;
	uint32_t c[MAX_N];
	// d list, i and j provided by the prover
	uint32_t d[MAX_N];
	uint32_t i;
	uint32_t j;
	// For each element in D, the prover supplies the corresponding index in C
	// For each element in C, the prover supplies the corresponding index in D
	// This proves that D is a permutation of C
	uint32_t ind_in_c[MAX_N];
	uint32_t ind_in_d[MAX_N];
};

struct Out {
	uint32_t d[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int i = input->i;
	int j = input->j;
	int n = input->n;
	int k, ci, indk;
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
	// C_j > C_i && forall k > i, C_k < C_i || C_k >= C_j
	int cj = c[j];
	int ck, tmp;
	assert_zero(cj <= ci);
	for (k = 0; k < MAX_N; k++) {
		ck = c[k];
		if (k > i && k < n) {

			tmp = (ck >= ci) * (ck < cj);
			assert_zero(tmp);
		}
	}

	// Verify that d is correct
	// D[0..i] = C[0..i]
	// D_i = C_j
	// D[i + 1..n] strictly increasing and is permutation of C[i..n] \ C_j
	for (k = 0; k < MAX_N; k++) {
		if (k < n) {
			if (k < i) assert_zero(input->d[k] != c[k]);
			else {
				if (k == i)
					assert_zero(input->d[k] != cj);
				// We don't need the second condition but pequin won't
				// compile the program without it
				else if (k < n - 1 && k < MAX_N - 1)
					assert_zero(input->d[k] >= input->d[k+1]);
				// D_k in C
				indk = input->ind_in_c[k];
				assert_zero(indk < i); assert_zero(indk >= n);
				assert_zero(input->d[k] == c[indk]);
				// C_k in D
				indk = input->ind_in_d[k];
				assert_zero(indk < i); assert_zero(indk >= n);
				assert_zero(c[k] == input->d[indk]);
			}
		}
	}
}
