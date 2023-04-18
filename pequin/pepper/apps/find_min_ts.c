#define MAX_N 10
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_N];
  int n[1];
};
struct Out {
  int B[MAX_N];
  int min;
};
typedef struct ghost_s {
	int values[1 + MAX_N + 1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[2] = {input->A, input->n};
	ghost_t ghost[1];
	int len[2] = {MAX_N, 1};
	exo_compute(public_info, len, ghost, 1);
	int min = ghost[0].values[0];
	int B[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		B[ITER1] = ghost[0].values[0 + 1 + ITER1];
	}
	int p = ghost[0].values[0 + 1 + MAX_N];
	int n = input->n[0];
	int accumErr = 0;
	int i4; for(i4 = 0; i4 < MAX_N; i4++){
		if(i4 < n) {
			if(min > slot( input->A, i4)) { accumErr++; }
			if(min == slot( input->A, i4)) {
				if(slot(B, i4) != 1) { accumErr++; }
			} else {
				if(slot(B, i4) != 0) { accumErr++; }
			}
		}
	}
	if(p < 0) { accumErr++; }
	if(p >= n) { accumErr++; }
	if(slot( input->A, p) != min) { accumErr++; }
	assert_zero(accumErr);
}
