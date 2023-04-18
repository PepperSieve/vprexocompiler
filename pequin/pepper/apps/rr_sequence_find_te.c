#define MAX_N 10
#define M 2
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
	int values[M * MAX_N + MAX_N + 1 + MAX_N];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[2] = {input->n, input->seq_0};
	ghost_t ghost[1];
	int len[2] = {1, 1};
	exo_compute(public_info, len, ghost, 1);
	uint32_t fseq[M * MAX_N];
	for (ITER1 = 0; ITER1 < M * MAX_N; ITER1++) {
		fseq[ITER1] = ghost[0].values[0 + ITER1];
	}
	uint32_t prev_k[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		prev_k[ITER1] = ghost[0].values[0 + M * MAX_N + ITER1];
	}
	uint32_t seq_last = ghost[0].values[0 + M * MAX_N + MAX_N];
	uint32_t seq[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		seq[ITER1] = ghost[0].values[0 + M * MAX_N + MAX_N + 1 + ITER1];
	}
	uint32_t n = input->n[0];
	uint32_t seq_0 = input->seq_0[0];
	uint32_t cur_seq = slot(seq, 0);
	assert_zero(cur_seq != seq_0);
	int j1; for(j1 = 0; j1 < M; j1++) {
		assert_zero(mat_slot( fseq, MAX_N, j1, 0 ) < 0);
		if(mat_slot( fseq, MAX_N, j1, 0 ) >= n) {
			assert_zero(f(j1, cur_seq) <= seq_last);
		} else {
			assert_zero(slot(seq, mat_slot( fseq, MAX_N, j1, 0 ) ) != f(j1, cur_seq));
		}
	}
	int k; for(k = 0; k < MAX_N-1; k++) {
		if(k+1 < n) {
			cur_seq = slot(seq, k+1);
			if(k+1 == n - 1) {
				assert_zero(cur_seq != seq_last);
			}
			assert_zero(slot(seq, k+1-1 ) >= cur_seq);
			assert_zero(slot(prev_k, k+1) < 0);
			assert_zero(slot(prev_k, k+1) >= k+1);
			uint32_t prev = slot( seq, slot( prev_k, k+1) );
			uint32_t prev_count = 0;
			int j2; for(j2 = 0; j2 < M; j2++) {
				if(f(j2, prev) == cur_seq) {
					prev_count = prev_count + 1;
				}
				assert_zero(mat_slot( fseq, MAX_N, j2, k+1 ) < 0);
				if(mat_slot( fseq, MAX_N, j2, k+1 ) >= n) {
					assert_zero(f(j2, cur_seq) <= seq_last);
				} else {
					assert_zero(slot(seq, mat_slot( fseq, MAX_N, j2, k+1 ) ) != f(j2, cur_seq));
				}
			}
			assert_zero(prev_count == 0);
		}
	}
}
