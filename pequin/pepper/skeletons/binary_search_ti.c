#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_N];
  int x;
  int l;
  int r;
};
struct Out {
  int ind;
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int x = input->x;
	int l = input->l;
	int r = input->r;
	int r1 = input->r;
	int l1 = input->l;
	int ind = -1;
	int tmp; for(tmp = 0; tmp < MAX_LOG; tmp++){
		if(ind == -1 && r1 > l1) {
			int mid = (l1 + r1) >> 1;
			int amid = slot( input->A, mid);
			if(amid > x) {
				r1 = mid;
			} else {
				l1 = mid + 1;
			}
			if(amid == x) {
				ind = mid;
			}
		}
	}
	output->ind = ind;
}
