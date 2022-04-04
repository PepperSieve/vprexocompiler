#include <stdint.h>

// N --> Number of elements in the sequence
// M --> Number of recurrence relations

#define f(i, x) (2 * i + 3) * x

struct In {
	uint32_t seq_0;
	uint32_t n;
};

struct Out {
	uint32_t seq[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int seq_0 = input->seq_0;
	int n = input->n;
	int x[M];
	int i[M];
	int j;
	output->seq[0] = seq_0;
	for (j = 0; j < M; j++) {
		i[j] = 0;
		x[j] = f(j, output->seq[0]);
	}

	int count;
	// Always ensure Fk(output->seq[ik]) > output->seq[count]
	for (count = 1; count < MAX_N; count++) {
		if (count < n) {

			// FIND_MIN
			output->seq[count] = x[0];
			for (j = 0; j < M; j++) {
				if (x[j] < output->seq[count]) {
					output->seq[count] = x[j];
				}
			}

			for (j = 0; j < M; j++) {
				if (x[j] == output->seq[count]) {
					i[j] += 1;
					x[j] = f(j, output->seq[i[j]]);
				}
			}
		}
	}
}
