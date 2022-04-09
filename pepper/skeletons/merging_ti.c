// Merge L sorted lists into one sorted list C
// Original L lists are stored in a 2-D array, A
// The length of each array is stored in an array, N

#include <stdint.h>

#define Arr(i, k) input->A[i * MAX_N + k]

struct In {
	uint32_t L;
	uint32_t N[MAX_L];
	uint32_t A[MAX_L * MAX_N];
};

struct Out {
	uint32_t B[MAX_L * MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int i[MAX_L];
	int L = input->L;
	int k, j, min, min_j, aji;
	// Compute the length of B
	int B_len = 0;
	for (k = 0; k < MAX_L; k++) {
		i[k] = 0;
		if (k < L) B_len += input->N[k];
	}
	// Use find min to compute each B[k]
	for (k = 0; k < MAX_L * MAX_N; k++) {
		if (k < B_len) {
			min = 2147483647;
			min_j = -1;
			for (j = 0; j < MAX_L; j++) {
				aji == Arr(j, i[j]);
				if (j < L && i[j] < input->N[j] && aji < min) {
					min = aji;
					min_j = j;
				}
			}
			output->B[k] = min;
			i[min_j]++;
		}
	}
}
