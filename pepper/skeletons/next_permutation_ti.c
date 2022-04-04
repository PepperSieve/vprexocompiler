#include <stdint.h>

struct In {
	uint32_t n;
	uint32_t c[MAX_N];
};

struct Out {
	uint32_t d[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int i = input->n - 2;
	int j = input->n - 1;
	int n = input->n;
	int k, lc;
	uint32_t tmp;

	// Don't want to modify c
	for (k = 0; k < MAX_N; k++) {
		if (k < n) output->d[k] = input->c[k];
	}
	
	int d_i = output->d[i + 1];
	int d_j;
	for (k = 0; k < MAX_N - 1; k++) {
		// Assume that d has a next permutation
		// This means that the while loop must terminate before i = 0
		d_j = output->d[i];
		if (d_j >= d_i) {
			i--;
			d_i = d_j;
		}
	}

	d_i = output->d[i];
	for (k = 0; k < MAX_N; k++) {
		// j must be greater than i
		if (output->d[j] <= d_i) j -= 1;
	}

	tmp = output->d[j];
	output->d[j] = output->d[i];
	output->d[i] = tmp;
	int p = i + 1;
	int q = n - 1;
	for (k = 0; k < MAX_N; k += 2) {
		if (p < q) {
			tmp = output->d[p];
			output->d[p] = output->d[q];
			output->d[q] = tmp;
			p++;
			q--;
		}
	}
}
