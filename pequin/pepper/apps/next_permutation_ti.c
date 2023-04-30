#define MAX_N 4
#define MAX_FAC 24
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int C[MAX_N];
  int n;
};
struct Out {
  int D[MAX_N];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int n = input->n;
	int CC[MAX_N];
	int D[MAX_N];
	int k1; for(k1 = 0; k1 < MAX_N; k1++){
		if(k1 < n) {
			slot(CC, k1) = slot( input->C, k1);
		}
	}
	int i = -1;
	int d_i = 0;
	int d_j = 0;
	int k2; for(k2 = 0; k2 < MAX_N-1; k2++){
		int tmp = MAX_N - (k2+1) - 1;
		if(i == -1 && tmp < n - 1) {
			if(slot(CC, tmp) < slot(CC, tmp+1)) {
				i = tmp;
				d_i = slot(CC, tmp);
			}
		}
	}
	int j = -1;
	int k3; for(k3 = 0; k3 < MAX_N-1; k3++){
		int tmp = MAX_N - (k3+1);
		if(j == -1 && tmp < n) {
			if(slot(CC, tmp) > d_i) {
				j = tmp;
				d_j = slot(CC, tmp);
			}
		}
	}
	slot(CC, i) = d_j;
	slot(CC, j) = d_i;
	int cor_ti = -1;
	int k4; for(k4 = 0; k4 < MAX_N; k4++){
		if(k4 < n) {
			if(k4 <= i) {
				cor_ti = k4;
			} else {
				cor_ti = n - k4 + i;
			}
			slot( D, k4) = slot(CC, cor_ti);
		}
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->D[ITER1] = D[ITER1];
	}
}
