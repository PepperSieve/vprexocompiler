#define MAX_N 10
#define MAX_FAC 3628800
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
	int accumErr = 0;
	int is_permutation = 0;
	int st[MAX_N];
	int cc[MAX_N];
	int k8; for(k8 = 0; k8 < MAX_N; k8++){
		slot( st, k8) = 0;
		slot( cc, k8) = slot( input->C, k8);
	}
	k8 = 1;
	int k9; for(k9 = 0; k9 < MAX_FAC; k9++){
		if(k8 < n) {
			if(slot( st, k8) < k8) {
				if(k8 % 2 == 0) {
					int tmp = slot( cc, 0);
					slot( cc, 0) = slot( cc, k8);
					slot( cc, k8) = tmp;
				} else {
					int tmp = slot( cc, slot( st, k8) );
					slot( cc, slot( st, k8) ) = slot( cc, k8);
					slot(cc, k8) = tmp;
				}
				int lte_c = -1;
				int ts_j = 0;
				int k10; for(k10 = 0; k10 < MAX_N; k10++){
					if(lte_c == -1 && k10 < n && slot( cc, k10) == slot( input->C, k10)) {
						ts_j = ts_j + 1;
					} else {
						lte_c = 0;
					}
				}
				if(ts_j == n || slot( cc, ts_j) < slot( input->C, ts_j)) {
					lte_c = 1;
				}
				int gte_d = -1;
				ts_j = 0;
				int k11; for(k11 = 0; k11 < MAX_N; k11++){
					if(gte_d == -1 && k11 < n && slot( cc, k11) == slot( D, k11)) {
						ts_j = ts_j + 1;
					} else {
						gte_d = 0;
					}
				}
				if(ts_j == n) {
					is_permutation = 1;
				}
				if(ts_j == n || slot( cc, j) > slot( D, j)) {
					gte_d = 1;
				}
				if(lte_c + gte_d - 1) { accumErr++; }
				slot( st, k8) = slot( st, k8) + 1;
				i = 1;
			} else {
				slot( st, k8) = 0;
				i = i + 1;
			}
		}
	}
	if(is_permutation - 1) { accumErr++; }
	assert_zero(accumErr);
}
