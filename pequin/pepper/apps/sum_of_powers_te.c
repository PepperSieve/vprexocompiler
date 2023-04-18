#define MAX_R 10
#define MAX_K 1
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
int pow(int x, int k) {
    int y = 1;
    int tmp;
    for (tmp = 0; tmp < MAX_K; tmp++) {
        if (k > 0) {
            y *= x;
            k--;
        }
    }
    return y;
}
struct In {
  int k[1];
  int r[1];
};
struct Out {
  int out_size;
  int y[MAX_R+1];
  int x[MAX_R+1];
};
typedef struct ghost_s {
	int values[1 + MAX_R+1 + 1 + MAX_R+1 + MAX_R+1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[2] = {input->k, input->r};
	ghost_t ghost[1];
	int len[2] = {1, 1};
	exo_compute(public_info, len, ghost, 1);
	int x2 = ghost[0].values[0];
	int cor_y[MAX_R+1];
	for (ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		cor_y[ITER1] = ghost[0].values[0 + 1 + ITER1];
	}
	int x1 = ghost[0].values[0 + 1 + MAX_R+1];
	int y[MAX_R+1];
	for (ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		y[ITER1] = ghost[0].values[0 + 1 + MAX_R+1 + 1 + ITER1];
	}
	int x[MAX_R+1];
	for (ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		x[ITER1] = ghost[0].values[0 + 1 + MAX_R+1 + 1 + MAX_R+1 + ITER1];
	}
	int k = input->k[0];
	int r = input->r[0];
	assert_zero(x1 < 0);
	if(x1 != 0) {
		assert_zero(2 * pow(x1 - 1, k) >= r);
	}
	assert_zero(2 * pow(x1, k) < r);
	assert_zero(pow(x2, k) > r);
	assert_zero(pow(x2 + 1, k) <= r);
	int count = 0;
	int j; for(j = 0; j < MAX_R+1; j++) {
		if(x1 + j <= x2) {
			int tmp1 = pow(x1 + j, k);
			int tmp2 = pow( slot(cor_y, j) , k);
			assert_zero(tmp1 + tmp2 > r);
			assert_zero(tmp1 + pow( slot(cor_y, j) + 1, k) <= r);
			if(tmp1 + tmp2 == r) {
				assert_zero(slot(x, count) != x1 + j);
				assert_zero(slot(y, count) != slot(cor_y, j));
				count = count + 1;
			}
		}
	}
	int out_size = count;
}
