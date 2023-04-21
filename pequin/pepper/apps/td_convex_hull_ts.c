#define MAX_N 20
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
	int values[MAX_N + MAX_N + MAX_N + MAX_N + MAX_N];
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
	int in_c[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		in_c[ITER1] = ghost[0].values[0 + MAX_N + MAX_N + MAX_N + ITER1];
	}
	int stack_y[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		stack_y[ITER1] = ghost[0].values[0 + MAX_N + MAX_N + MAX_N + MAX_N + ITER1];
	}
	int n = input->n[0];
	int accumErr = 0;
	int x0_ts = slot( input->X, 0);
	int y0_ts = slot( input->Y, 0);
	if(x0_ts - slot(stack_x, 0)) { accumErr++; }
	if(y0_ts - slot(stack_y, 0)) { accumErr++; }
	int count_ts = 0;;
	int k2; for(k2 = 0; k2 < MAX_N-1; k2++){
		if(k2+1 < n) {
			int xi = slot( input->X, k2+1);
			int yi = slot( input->Y, k2+1);
			int last = k2;
			int next = k2 + 2;;
			if(next == MAX_N) {
				next = 0;
			}
			int k3; for(k3 = 0; k3 < MAX_N; k3++){
				if(slot(in_c, last) != 1) {
					last = last - 1;
				}
				if(next < n && slot(in_c, next) != 1) {
					next = next + 1;
				}
			}
			if(next == n) {
				next = 0;
			}
			int last_x_ts = slot( input->X, last);
			int last_y_ts = slot( input->Y, last);
			int next_x_ts = slot( input->X, next);
			int next_y_ts = slot( input->Y, next);
			int prod_ts = X_PROD(last_x_ts, last_y_ts, xi, yi, next_x_ts, next_y_ts);
			if(slot(in_c, k2+1) == 1) {
				count_ts = count_ts + 1;
				if(slot(stack_x, count_ts) - xi) { accumErr++; }
				if(slot(stack_y, count_ts) - yi) { accumErr++; }
				if(prod_ts <= 0) { accumErr++; }
			} else {
				if(prod_ts >= 0) { accumErr++; }
			}
		}
	}
	int ss = count_ts + 1;
	assert_zero(accumErr);
}
