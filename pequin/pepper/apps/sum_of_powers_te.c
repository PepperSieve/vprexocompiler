#define MAX_R 20
#define MAX_K 4
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
  int x[MAX_R+1];
  int y[MAX_R+1];
};
typedef struct ghost_s {
	int values[1 + 1 + 1 + MAX_R+1 + MAX_R+1 + MAX_R+1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[2] = {input->k, input->r};
	ghost_t ghost[1];
	int len[2] = {1, 1};
	exo_compute(public_info, len, ghost, 1);
	int x2 = ghost[0].values[0];
	int out_size = ghost[0].values[0 + 1];
	int x1 = ghost[0].values[0 + 1 + 1];
	int cor_y[MAX_R+1];
	for (ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		cor_y[ITER1] = ghost[0].values[0 + 1 + 1 + 1 + ITER1];
	}
	int x[MAX_R+1];
	for (ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		x[ITER1] = ghost[0].values[0 + 1 + 1 + 1 + MAX_R+1 + ITER1];
	}
	int y[MAX_R+1];
	for (ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		y[ITER1] = ghost[0].values[0 + 1 + 1 + 1 + MAX_R+1 + MAX_R+1 + ITER1];
	}
	int k = input->k[0];
	int r = input->r[0];
	int accumErr = 0;
	if(x1 < 0) { accumErr++; }
	if(x1 != 0) {
		if(2 * pow(x1 - 1, k) >= r) { accumErr++; }
	}
	if(2 * pow(x1, k) < r) { accumErr++; }
	if(pow(x2, k) > r) { accumErr++; }
	if(pow(x2 + 1, k) <= r) { accumErr++; }
	int count_te = 0;
	int j; for(j = 0; j < MAX_R+1; j++) {
		if(x1 + j <= x2) {
			int tmp1_te = pow(x1 + j, k);
			int tmp2_te = pow( slot(cor_y, j) , k);
			if(tmp1_te + tmp2_te > r) { accumErr++; }
			if(tmp1_te + pow( slot(cor_y, j) + 1, k) <= r) { accumErr++; }
			if(tmp1_te + tmp2_te == r) {
				if(slot(x, count_te) != x1 + j) { accumErr++; }
				if(slot(y, count_te) != slot(cor_y, j)) { accumErr++; }
				count_te = count_te + 1;
			}
		}
	}
	if(out_size - count_te) { accumErr++; }
	assert_zero(accumErr);
}
