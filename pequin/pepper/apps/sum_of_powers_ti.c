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
  int k;
  int r;
};
struct Out {
  int out_size;
  int x[MAX_R+1];
  int y[MAX_R+1];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int k = input->k;
	int r = input->r;
	int cur_x = 0;
	int cur_y = 0;
	int x[MAX_R+1];
	int y[MAX_R+1];
	int k1; for(k1 = 0; k1 < MAX_R; k1++){
		if(pow(cur_x, k) + pow(cur_y, k) < r) {
			cur_x = cur_x + 1;
			cur_y = cur_y + 1;
		}
	}
	int i = 0;
	int tmp2; for(tmp2 = 0; tmp2 < 2*(MAX_R+1); tmp2++){
		if(pow(cur_x, k) <= r) {
			if(pow(cur_x, k) + pow(cur_y, k) > r) {
				cur_y = cur_y - 1;
			} else {
				if(pow(cur_x, k) + pow(cur_y, k) == r) {
					slot(x, i) = cur_x;
					slot(y, i) = cur_y;
					i = i + 1;
					cur_x = cur_x + 1;
				} else {
					cur_x = cur_x + 1;
				}
			}
		}
	}
	int out_size = i;
	output->out_size = out_size;
	for(ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		output->x[ITER1] = x[ITER1];
	}
	for(ITER1 = 0; ITER1 < MAX_R+1; ITER1++) {
		output->y[ITER1] = y[ITER1];
	}
}
