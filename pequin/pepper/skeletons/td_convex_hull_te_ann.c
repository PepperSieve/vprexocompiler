#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
#define X_PROD(x1, y1, x2, y2, x3, y3) (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
struct In {
  int n[1];
  int Y[MAX_N];
  int X[MAX_N];
};
struct Out {
  int stack_y[MAX_N];
  int ss;
  int stack_x[MAX_N];
};
typedef struct ghost_s {
	int values[MAX_N + MAX_N + MAX_N + MAX_N];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[3] = {input->n, input->Y, input->X};
	ghost_t ghost[1];
	int len[3] = {1, MAX_N, MAX_N};
	exo_compute(public_info, len, ghost, 1);
	int NY[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		NY[ITER1] = ghost[0].values[0 + ITER1];
	}
	int NX[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		NX[ITER1] = ghost[0].values[0 + MAX_N + ITER1];
	}
	int stack_x[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		stack_x[ITER1] = ghost[0].values[0 + MAX_N + MAX_N + ITER1];
	}
	int stack_y[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		stack_y[ITER1] = ghost[0].values[0 + MAX_N + MAX_N + MAX_N + ITER1];
	}
	int n = input->n[0];
	int x0 = slot( input->X, 0);
	int y0 = slot( input->Y, 0);
	int next_x = slot(NX, 0);
	int next_y = slot(NY, 0);
	int last_x = x0;
	int last_y = y0;
	assert_zero(x0 - slot(stack_x, 0));
	assert_zero(y0 - slot(stack_y, 0));
	int count = 0;
	int k1; for(k1 = 0; k1 < MAX_N-1; k1++) {
		if(k1+1 < n) {
			int xi = slot( input->X, k1+1);
			int yi = slot( input->Y, k1+1);
			if(xi == next_x) {
				count = count + 1;
				// assert_zero(slot(stack_x, count) - xi);
				// assert_zero(slot(stack_y, count) - yi);
				assert_zero(yi - next_y);
				next_x = slot(NX, k1+1);
				next_y = slot(NY, k1+1);
				int prod = X_PROD(last_x, last_y, xi, yi, next_x, next_y);
				assert_zero(prod <= 0);
				last_x = xi;
				last_y = yi;
			} else {
				int prod = X_PROD(last_x, last_y, xi, yi, next_x, next_y);
				assert_zero(prod >= 0);
			}
		}
	}
	assert_zero(x0 - next_x);
	assert_zero(y0 - next_y);
}
