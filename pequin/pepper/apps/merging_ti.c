#define MAX_N 10
#define MAX_L 14
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_L * MAX_N];
  int L;
  int N[MAX_L];
};
struct Out {
  int B[MAX_L*MAX_N];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int L = input->L;
	int curr[MAX_L];
	int min = 0;
	int kstar = 0;
	int aki = 0;
	int B[MAX_N*MAX_L];
	// Compute the length of B;
	int B_len_ti = 0;
	int k1; for(k1 = 0; k1 < MAX_L; k1++){
		slot(curr, k1) = 0;
		if(k1 < L) {
			B_len_ti = B_len_ti + slot( input->N, k1);
		}
	}
	int k2; for(k2 = 0; k2 < MAX_L*MAX_N; k2++){
		if(k2 < B_len_ti) {
			min = 2147483647;
			kstar = -1;
			int k; for(k = 0; k < MAX_L; k++){
				aki = mat_slot( input->A, MAX_N, k, slot(curr, k) );
				if(k < L && slot(curr, k) < slot( input->N, k) && aki < min) {
					min = aki;
					kstar = k;
				}
			}
			slot(B, k2) = min;
			slot(curr, kstar) = slot(curr, kstar) + 1;
		}
	}
	for(ITER1 = 0; ITER1 < MAX_L*MAX_N; ITER1++) {
		output->B[ITER1] = B[ITER1];
	}
}
