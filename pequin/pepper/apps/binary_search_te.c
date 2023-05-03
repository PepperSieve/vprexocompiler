#define MAX_N 5120
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
	if(l == r || x < slot( input->A, l) || slot( input->A, r - 1) < x) {
		assert_zero(ind != -1);
	} else {
		if(ind == -1) {
			assert_zero(sup < l);
			assert_zero(sup >= r - 1);
			int tmp1 = slot( input->A, sup);
			assert_zero(tmp1 >= x);
			int tmp2 = slot( input->A, sup + 1);
			assert_zero(x >= tmp2);
		} else {
			assert_zero(ind < l);
			assert_zero(ind >= r);
			assert_zero(x != slot( input->A, ind));
		}
	}
}
