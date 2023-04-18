#define MAX_N 10
#define M 2
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
	int j1; for(j1 = 0; j1 < M; j1++){
		slot(i, j1) = 0;
		slot(x, j1) = f(j1, slot(seq, 0) );
	}
	int count; for(count = 0; count < MAX_N-1; count++){
		if(count+1 < n) {
			slot(seq, count+1) = slot(x, 0);
			int j2; for(j2 = 0; j2 < M; j2++){
				if(slot(x, j2) < slot(seq, count+1)) {
					slot(seq, count+1) = slot(x, j2);
				}
			}
			int j3; for(j3 = 0; j3 < M; j3++){
				if(slot(x, j3) == slot(seq, count+1)) {
					slot(i, j3) = slot(i, j3) + 1;
					slot(x, j3) = f(j3, slot(seq, slot(i, j3) ) );;
				}
			}
		}
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->seq[ITER1] = seq[ITER1];
	}
}
