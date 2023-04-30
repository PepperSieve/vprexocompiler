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
	uint32_t cur_seq_ts = slot(seq, 0);
	if(cur_seq_ts != seq_0) { accumErr++; }
	uint32_t k6; for(k6 = 0; k6 < MAX_N-1; k6++){
		if(k6 + 1 < n) {
			cur_seq_ts = slot( seq, k6+1);
			uint32_t pk = slot( prev_k, k6+1);
			uint32_t pj = slot( prev_j, k6+1);
			uint32_t seq_pk = slot( seq, pk);
			if(pk < 0) { accumErr++; }
			if(pk >= k6+1) { accumErr++; }
			if(pj < 0) { accumErr++; }
			if(pj >= M) { accumErr++; }
			if(f(pj, seq_pk) - cur_seq_ts != 0) { accumErr++; }
			uint32_t k7; for(k7 = 0; k7 < M; k7++){
				uint32_t k8; for(k8 = 0; k8 < MAX_N; k8++){
					if(k8 < k6) {
						uint32_t fjk8 = f(k7, slot( seq, k8) );
						if(fjk8 > slot(seq, k6) && fjk8 < cur_seq_ts) { accumErr++; }
					}
				}
			}
		}
	}
	assert_zero(accumErr);
}
