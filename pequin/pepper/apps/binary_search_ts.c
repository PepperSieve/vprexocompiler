#define MAX_N 20
#define MAX_LOG 5
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_N];
  int x[1];
  int l[1];
  int r[1];
};
struct Out {
  int ind;
};
typedef struct ghost_s {
	int values[1 + 1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[4] = {input->A, input->x, input->l, input->r};
	ghost_t ghost[1];
	int len[4] = {MAX_N, 1, 1, 1};
	exo_compute(public_info, len, ghost, 1);
	int sup = ghost[0].values[0];
	int ind = ghost[0].values[0 + 1];
	int x = input->x[0];
	int l = input->l[0];
	int r = input->r[0];
	int accumErr = 0;
	if(ind < l && ind != -1) { accumErr++; }
	if(ind >= r) { accumErr++; }
	if(ind != -1) {
		if(slot( input->A, ind) != x) { accumErr++; }
	} else {
		int tmp; for(tmp = 0; tmp < MAX_N; tmp++){
			if(tmp >= l) {
				if(tmp < r) {
					if(slot( input->A, tmp) == x) { accumErr++; }
				}
			}
		}
	}
	assert_zero(accumErr);
}
