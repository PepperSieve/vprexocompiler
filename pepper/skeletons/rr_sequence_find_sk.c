#include <stdint.h>

// N --> Number of elements in the sequence
// M --> Number of recurrence relations
#define MAX_N 50
#define M 6

#define F0(x) 2 * x
#define F1(x) 3 * x
#define F2(x) 5 * x
#define F3(x) 7 * x
#define F4(x) 11 * x
#define F5(x) 13 * x

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
	for (j = 0; j < M; j++) i[j] = 0;
	x[0] = F0(output->seq[i[0]]);
	x[1] = F1(output->seq[i[1]]);
	x[2] = F2(output->seq[i[2]]);
	x[3] = F3(output->seq[i[3]]);
	x[4] = F4(output->seq[i[4]]);
	x[5] = F5(output->seq[i[5]]);

	int count;
	// Always ensure Fk(output->seq[ik]) > output->seq[count]
	for (count = 1; count < MAX_N; count++) {
		if (count < n) {

			// Tiny FIND_MIN structure to facilitate adding more RRs
			output->seq[count] = x[0];
			for (j = 0; j < M; j++) {
				if (x[j] < output->seq[count]) {
					output->seq[count] = x[j];
				}
			}

			if (x[0] == output->seq[count]) {
				i[0] += 1;
				x[0] = F0(output->seq[i[0]]);
			}
			if (x[1] == output->seq[count]) {
				i[1] += 1;
				x[1] = F1(output->seq[i[1]]);
			}
			if (x[2] == output->seq[count]) {
				i[2] += 1;
				x[2] = F2(output->seq[i[2]]);
			}
			if (x[3] == output->seq[count]) {
				i[3] += 1;
				x[3] = F3(output->seq[i[3]]);
			}
			if (x[4] == output->seq[count]) {
				i[4] += 1;
				x[4] = F4(output->seq[i[4]]);
			}
			if (x[5] == output->seq[count]) {
				i[5] += 1;
				x[5] = F5(output->seq[i[5]]);
			}

		}
	}
}
