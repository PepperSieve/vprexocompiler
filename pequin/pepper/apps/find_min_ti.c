#define MAX_N 10
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_N];
  int n;
};
struct Out {
  int B[MAX_N];
  int min;
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int n = input->n;
	int min = slot( input->A, 0);
	int p = 0;
	int i1; for(i1 = 0; i1 < MAX_N; i1++){
		if(i1 < n) {
			if(slot( input->A, i1) < min) {
				min = slot( input->A, i1);
				p = i1;
			}
		}
	}
	int B[MAX_N];
	int i2; for(i2 = 0; i2 < MAX_N; i2++){
		if(i2 < n) {
			if(slot( input->A,i2) == min) {
				slot(B,i2) = 1;
			} else {
				slot(B,i2) = 0;
			}
		}
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->B[ITER1] = B[ITER1];
	}
	output->min = min;
}
