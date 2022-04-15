#include <stdint.h>

struct In {
	uint32_t M;
	uint32_t N;
	uint32_t pat[MAX_M];
	uint32_t txt[MAX_N];
};

struct Out {
	uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
	int M = input->M;
	int N = input->N;

	int lps[MAX_M];

	int len = 0;
	lps[0] = 0;

	int i = 1;
	int k;
	for (k = 0; k < MAX_M * 2; k++) {
		if (i < M) {
			if ((input->pat)[i] == (input->pat)[len]) {
				len++;
				lps[i] = len;
				i++;
			} else {
				if (len != 0) {
					len = lps[len - 1];
				} else {
					lps[i] = 0;
					i++;
				}
			}
		}
	}

	i = 0;
	int j = 0;
	int found = 0;
	for (k = 0; k < MAX_N * 2; k++) {
		if (i < N) {
			// We find a solution
			if (j == M) {
				output->ind = i - M;
				found = 1;
				i = N + 1;
			} 
			// Case of a mismatch
			else if ((input->pat)[j] != (input->txt)[i]) {
				if (j != 0) {
					j = lps[j];
				}
				else i++;
			}
			// Case of a match
			else {
				j++;
				i++;
			}
		}
	}
	if (found == 0) output->ind = N;
}
