#include <stdint.h>

#define Arr(i, k) input->A[i * MAX_N + k]

struct In {
	uint32_t L;
	uint32_t N[MAX_L];
	uint32_t A[MAX_L * MAX_N];
	// the merged list c provided by the prover
	uint32_t C[MAX_L * MAX_N];
};

struct Out {
	uint32_t C[MAX_L * MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int L = input->L;
	int i, j, k, nk, akj;
	int c_len = 0;
	for (k = 0; k < MAX_L; k++) {
		if (k < L) c_len += input->N[k];
	}
	i = 0;
	for (i = 0; i < MAX_L * MAX_N; i++) {
		if (i < c_len) {
			// Assert c is increasing
			assert_zero(i != 0 && input->C[i - 1] >= input->C[i]);
			// Assert we can find correspondence of c
			int foundj = 0;
			for (k = 0; k < MAX_L; k++) {
				if (k < L) {
					nk = input->N[k];
					for (j = 0; j < MAX_N; j++) {
						if (j < nk) {
							akj = Arr(k, j);
							if (input->C[i] == akj) {foundj = 1;}
						}
					}
				}
			}
			assert_zero(foundj - 1);
		}
	}
}
