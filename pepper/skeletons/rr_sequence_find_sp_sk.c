#include <stdint.h>

// N --> Number of elements in the sequence
// M --> Number of recurrence relations
#define MAX_N 50
#define M 6

#define f0(x) 2 * x
#define f1(x) 3 * x
#define f2(x) 5 * x
#define f3(x) 7 * x
#define f4(x) 11 * x
#define f5(x) 13 * x

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
	uint32_t f0x[MAX_N];
	uint32_t f1x[MAX_N];
	uint32_t f2x[MAX_N];
	uint32_t f3x[MAX_N];
	uint32_t f4x[MAX_N];
	uint32_t f5x[MAX_N];
};

struct Out {
	uint32_t seq[MAX_N];
};

void compute(struct In *input, struct Out *output) {
	int seq_0 = input->seq_0;
	int n = input->n;
	int seq_last = input->seq_last;
	int i, prev;
	int seq = input->seq[0];
	assert_zero(seq - seq_0);
	assert_zero(input->f0x[0] < 0); assert_zero(input->f0x[0] > n);
	assert_zero(input->f1x[0] < 0); assert_zero(input->f1x[0] > n);
	assert_zero(input->f2x[0] < 0); assert_zero(input->f2x[0] > n);
	
	assert_zero(input->f3x[0] < 0); assert_zero(input->f3x[0] > n);
	assert_zero(input->f4x[0] < 0); assert_zero(input->f4x[0] > n);
	assert_zero(input->f5x[0] < 0); assert_zero(input->f5x[0] > n);

	if (input->f0x[0] == n) assert_zero(f0(seq) <= seq_last);
	else assert_zero(input->seq[input->f0x[0]] - f0(seq));
	if (input->f1x[0] == n) assert_zero(f1(seq) <= seq_last);
	else assert_zero(input->seq[input->f1x[0]] - f1(seq));
	if (input->f2x[0] == n) assert_zero(f2(seq) <= seq_last);
	else assert_zero(input->seq[input->f2x[0]] - f2(seq));

	if (input->f3x[0] == n) assert_zero(f3(seq) <= seq_last);
	else assert_zero(input->seq[input->f3x[0]] - f3(seq));
	if (input->f4x[0] == n) assert_zero(f4(seq) <= seq_last);
	else assert_zero(input->seq[input->f4x[0]] - f4(seq));
	if (input->f5x[0] == n) assert_zero(f5(seq) <= seq_last);
	else assert_zero(input->seq[input->f5x[0]] - f5(seq));

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
			// assert_zero(f0(prev) != seq && f1(prev) != seq && f2(prev) != seq);
			assert_zero(f0(prev) != seq && f1(prev) != seq && f2(prev) != seq
				&& f3(prev) != seq && f4(prev) != seq && f5(prev) != seq);

			// verify f0x, f1x, etc.
			assert_zero(input->f0x[i] <= i); assert_zero(input->f0x[i] > n);
			assert_zero(input->f1x[i] <= i); assert_zero(input->f1x[i] > n);
			assert_zero(input->f2x[i] <= i); assert_zero(input->f2x[i] > n);
			
			assert_zero(input->f3x[i] <= i); assert_zero(input->f3x[i] > n);
			assert_zero(input->f4x[i] <= i); assert_zero(input->f4x[i] > n);
			assert_zero(input->f5x[i] <= i); assert_zero(input->f5x[i] > n);

			if (input->f0x[i] == n) assert_zero(f0(seq) <= seq_last);
			else assert_zero(input->seq[input->f0x[i]] - f0(seq));
			if (input->f1x[i] == n) assert_zero(f1(seq) <= seq_last);
			else assert_zero(input->seq[input->f1x[i]] - f1(seq));
			if (input->f2x[i] == n) assert_zero(f2(seq) <= seq_last);
			else assert_zero(input->seq[input->f2x[i]] - f2(seq));
		
			if (input->f3x[i] == n) assert_zero(f3(seq) <= seq_last);
			else assert_zero(input->seq[input->f3x[i]] - f3(seq));
			if (input->f4x[i] == n) assert_zero(f4(seq) <= seq_last);
			else assert_zero(input->seq[input->f4x[i]] - f4(seq));
			if (input->f5x[i] == n) assert_zero(f5(seq) <= seq_last);
			else assert_zero(input->seq[input->f5x[i]] - f5(seq));
		}
	}
}
