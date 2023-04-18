#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int C[MAX_N];
  int n[1];
};
struct Out {
  int D[MAX_N];
};
typedef struct ghost_s {
	int values[1 + 1 + MAX_N + MAX_N + 1 + 1 + 1 + 1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[2] = {input->C, input->n};
	ghost_t ghost[1];
	int len[2] = {MAX_N, 1};
	exo_compute(public_info, len, ghost, 1);
	int cjp1 = ghost[0].values[0];
	int cip1 = ghost[0].values[0 + 1];
	int CC[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		CC[ITER1] = ghost[0].values[0 + 1 + 1 + ITER1];
	}
	int D[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		D[ITER1] = ghost[0].values[0 + 1 + 1 + MAX_N + ITER1];
	}
	int i = ghost[0].values[0 + 1 + 1 + MAX_N + MAX_N];
	int ci = ghost[0].values[0 + 1 + 1 + MAX_N + MAX_N + 1];
	int cj = ghost[0].values[0 + 1 + 1 + MAX_N + MAX_N + 1 + 1];
	int j = ghost[0].values[0 + 1 + 1 + MAX_N + MAX_N + 1 + 1 + 1];
	int n = input->n[0];
	int k5; for(k5 = 0; k5 < MAX_N; k5++) {
		if(k5 < n && k5 != i && k5 != j) {
			assert_zero(slot(CC, k5) != slot( input->C, k5));
		}
	}
	assert_zero(0 > i);
	assert_zero(i >= j);
	assert_zero(j >= n);
	assert_zero(ci >= cip1);
	assert_zero(cj <= ci);
	if(j != n - 1) {
		assert_zero(cjp1 >= ci);
	}
	int k6; for(k6 = 0; k6 < MAX_N-1; k6++) {
		if(k6+1 == i) {
			assert_zero(slot( input->C, k6+1) - ci);
			assert_zero(slot( CC, k6+1) - cj);
		}
		if(k6+1 == i + 1) {
			assert_zero(slot( input->C, k6+1) - cip1);
		}
		if(k6+1 == j) {
			assert_zero(slot( input->C, k6+1) - cj);
			assert_zero(slot( CC, k6+1) - ci);
		}
		if(k6+1 == j + 1 && k6+1 != n) {
			assert_zero(slot( input->C, k6+1) - cjp1);
		}
	}
	int cor_te = -1;
	int k7; for(k7 = 0; k7 < MAX_N; k7++) {
		if(k7 < n) {
			if(k7 <= i) {
				cor_te = k7;
			} else {
				cor_te = n - k7 + i;
			}
			assert_zero(slot(D, k7) != slot(CC, cor_te));
			if(k7 != 0 && k7 - 1 > i) {
				assert_zero(slot( input->C, k7-1) <= slot( input->C, k7));
			}
		}
	}
}
