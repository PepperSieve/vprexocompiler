#define MAX_N 10
#define M 4
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
#define f(i, x) (2 * i + 3) * x
struct In {
  int n;
  int seq_0;
};
struct Out {
  int seq[MAX_N];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int n = input->n;
	int seq_0 = input->seq_0;
	int x[M];
	int i[M];
	int seq[MAX_N];
	slot(seq, 0) = seq_0;
	int k1; for(k1 = 0; k1 < M; k1++){
		slot(i, k1) = 0;
		slot(x, k1) = f(k1, slot(seq, 0) );
	}
	int count; for(count = 0; count < MAX_N-1; count++){
		if(count+1 < n) {
			slot(seq, count+1) = slot(x, 0);
			int k2; for(k2 = 0; k2 < M; k2++){
				if(slot(x, k2) < slot(seq, count+1)) {
					slot(seq, count+1) = slot(x, k2);
				}
			}
			int k3; for(k3 = 0; k3 < M; k3++){
				if(slot(x, k3) == slot(seq, count+1)) {
					slot(i, k3) = slot(i, k3) + 1;
					slot(x, k3) = f(k3, slot(seq, slot(i, k3) ) );
				}
			}
		}
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->seq[ITER1] = seq[ITER1];
	}
}
