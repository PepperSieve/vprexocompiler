#include <stdint.h>

// N --> Number of elements in the sequence
// M --> Number of recurrence relations

#define f(i, x) (2 * i + 3) * x
#define fLOAD(i, x) input->f_x[i * MAX_N + x]

// This verification method is derived from Dijkstra's observation: If x is in the sequence, so does f1(x), f2(x), ...
// The following would hold:
// A. The sequence is monotone increasing
// B. For all seq[i], if seq[i] != seq_0, exists p such that f1(p) == seq[i] || f2(p) == seq[i] || ...
// 			  	 if f1(seq[i]) < bound, f1(seq[i]) in sequence, etc.

struct In {
	uint32_t seq_0;
	uint32_t n;
	// Actual result seq provided by the prover
	uint32_t seq[MAX_N];
	// seq_last records the value of seq[n - 1], provided by the prover
	uint32_t seq_last;
	// Supplimentary prev, f0x, f1x, f2x provided by the prover
	// f0x, f1x, f2x records down the index of f0(x), f1(x), f2(x)
	// If they are out-of-bound, set them to n
	uint32_t prev[MAX_N];
	uint32_t f_x[M * MAX_N];
};

struct Out {
	uint32_t seq[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int seq_0 = input->seq_0;
	int n = input->n;
	int seq_last = input->seq_last;
	int i, prev, j, prev_count;
	int seq = input->seq[0];
	assert_zero(seq - seq_0);
	for (j = 0; j < M; j++) {
		assert_zero(fLOAD(j, 0) < 0);
		if (fLOAD(j, 0) >= n)
			assert_zero(f(j, seq) <= seq_last);
		else
			assert_zero(input->seq[fLOAD(j, 0)] - f(j, seq));
	}

	for (i = 1; i < MAX_N; i++) {
		if (i < n) {
			seq = input->seq[i];
			// verify seq_last
			if (i == n - 1) assert_zero(seq - seq_last);
			
			// monotone increasing
			assert_zero(input->seq[i - 1] >= seq);
			
			// Verify prev, which must exist for i >= 1
			assert_zero(input->prev[i] < 0 || input->prev[i] >= i);
			prev = input->seq[input->prev[i]];

			prev_count = 0;
			for (j = 0; j < M; j++) {
				if (f(j, prev) == seq) prev_count++;
				assert_zero(fLOAD(j, i) <= i);
				if (fLOAD(j, i) >= n)
					assert_zero(f(j, seq) <= seq_last);
				else
					assert_zero(input->seq[fLOAD(j, i)] - f(j, seq));
			}
			// one of f(j, prev) must equal to seq
			assert_zero(prev_count == 0);
		}
	}
}
