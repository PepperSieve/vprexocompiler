#include <stdint.h>

#define Arr(i, k) input->A[i * MAX_N + k]

struct In {
	uint32_t L;
	uint32_t N[MAX_L];
	uint32_t A[MAX_L * MAX_N];
	// the merged list c provided by the prover
	uint32_t c[MAX_L * MAX_N];
	// suppliment x and k witness list
	// c[i] == a[x][k]
	uint32_t x[MAX_L * MAX_N];
	uint32_t k[MAX_L * MAX_N];
};

struct Out {
	uint32_t c[MAX_L * MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int L = input->L;
	int i, k, xi, ci, ni;
	int c_len = 0;
	for (k = 0; k < MAX_L; k++) {
		if (k < L) c_len += input->N[k];
	}
	for (i = 0; i < MAX_L * MAX_N; i++) {
		if (i < c_len) {
			// Assert c is increasing
			assert_zero(i != 0 && input->c[i - 1] >= input->c[i]);
			// Assert we can find correspondence of c
			xi = input->x[i];
			assert_zero(xi < 0);
			assert_zero(xi >= L);
			k = input->k[i];
			assert_zero(k < 0);
			ni = input->N[xi];
			assert_zero(k >= ni);
			ci = input->c[i];
			assert_zero(ci != Arr(xi, k));
		}
	}
}
