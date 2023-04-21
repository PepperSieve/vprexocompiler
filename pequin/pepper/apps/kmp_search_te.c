#define MAX_N 20
#define MAX_M 4
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int PAT[MAX_M];
  int M[1];
  int TXT[MAX_N];
  int N[1];
};
struct Out {
  int ind;
};
typedef struct ghost_s {
	int values[MAX_N + 1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[4] = {input->PAT, input->M, input->TXT, input->N};
	ghost_t ghost[1];
	int len[4] = {MAX_M, 1, MAX_N, 1};
	exo_compute(public_info, len, ghost, 1);
	int B[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		B[ITER1] = ghost[0].values[0 + ITER1];
	}
	int ind = ghost[0].values[0 + MAX_N];
	int M = input->M[0];
	int N = input->N[0];
	int accumErr = 0;
	if(ind > N - M + 1) { accumErr++; }
	int k3; for(k3 = 0; k3 < MAX_N; k3++) {
		if(k3 < ind) {
			int b_i = slot(B, k3);
			if(b_i >= M) { accumErr++; }
			int t = slot( input->TXT, k3+b_i);
			int p = slot( input->PAT, b_i);
			if(t == p) { accumErr++; }
		}
	}
	if(ind != N - M + 1) {
		int k4; for(k4 = 0; k4 < MAX_M; k4++) {
			if(k4 < M) {
				int t = slot( input->TXT, ind+k4);
				int p = slot( input->PAT, k4);
				if(t != p) { accumErr++; }
			}
		}
	} else {
		ind = N;
	}
	assert_zero(accumErr);
}
