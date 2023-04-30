#define MAX_N 10
#define M 4
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
#define f(i, x) (2 * i + 3) * x
struct In {
  int n[1];
  int seq_0[1];
};
struct Out {
  int seq[MAX_N];
};
typedef struct ghost_s {
	int values[1 + M * MAX_N*M + MAX_N + MAX_N + MAX_N];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[2] = {input->n, input->seq_0};
	ghost_t ghost[1];
	int len[2] = {1, 1};
	exo_compute(public_info, len, ghost, 1);
	uint32_t seq_last = ghost[0].values[0];
	uint32_t fseq[M * MAX_N*M];
	for (ITER1 = 0; ITER1 < M * MAX_N*M; ITER1++) {
		fseq[ITER1] = ghost[0].values[0 + 1 + ITER1];
	}
	uint32_t prev_j[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		prev_j[ITER1] = ghost[0].values[0 + 1 + M * MAX_N*M + ITER1];
	}
	uint32_t seq[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		seq[ITER1] = ghost[0].values[0 + 1 + M * MAX_N*M + MAX_N + ITER1];
	}
	uint32_t prev_k[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		prev_k[ITER1] = ghost[0].values[0 + 1 + M * MAX_N*M + MAX_N + MAX_N + ITER1];
	}
	uint32_t n = input->n[0];
	uint32_t seq_0 = input->seq_0[0];
	int accumErr = 0;
	uint32_t cur_seq_te = slot(seq, 0);
	if(cur_seq_te != seq_0) { accumErr++; }
	int k4; for(k4 = 0; k4 < M; k4++) {
		if(mat_slot( fseq, MAX_N, k4, 0 ) < 0) { accumErr++; }
		if(mat_slot( fseq, MAX_N, k4, 0 ) >= n) {
			if(f(k4, cur_seq_te) <= seq_last) { accumErr++; }
		} else {
			if(slot(seq, mat_slot( fseq, MAX_N, k4, 0 ) ) != f(k4, cur_seq_te)) { accumErr++; }
		}
	}
	int k; for(k = 0; k < MAX_N-1; k++) {
		if(k+1 < n) {
			cur_seq_te = slot(seq, k+1);
			if(k+1 == n - 1) {
				if(cur_seq_te != seq_last) { accumErr++; }
			}
			if(slot(seq, k+1-1 ) >= cur_seq_te) { accumErr++; }
			if(slot(prev_k, k+1) < 0) { accumErr++; }
			if(slot(prev_k, k+1) >= k+1) { accumErr++; }
			uint32_t prev = slot( seq, slot( prev_k, k+1) );
			uint32_t prev_count = 0;
			int k5; for(k5 = 0; k5 < M; k5++) {
				if(f(k5, prev) == cur_seq_te) {
					prev_count = prev_count + 1;
				}
				if(mat_slot( fseq, MAX_N, k5, k+1 ) < 0) { accumErr++; }
				if(mat_slot( fseq, MAX_N, k5, k+1 ) >= n) {
					if(f(k5, cur_seq_te) <= seq_last) { accumErr++; }
				} else {
					if(slot(seq, mat_slot( fseq, MAX_N, k5, k+1 ) ) != f(k5, cur_seq_te)) { accumErr++; }
				}
			}
			if(prev_count == 0) { accumErr++; }
		}
	}
	assert_zero(accumErr);
}
