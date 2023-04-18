#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
#define X_PROD(x1, y1, x2, y2, x3, y3) (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
struct In {
  int n;
  int Y[MAX_N];
  int X[MAX_N];
};
struct Out {
  int stack_y[MAX_N];
  int ss;
  int stack_x[MAX_N];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int n = input->n;
	int stack_x[MAX_N];
	int stack_y[MAX_N];
	int x1 = slot( input->X, 0);
	slot( stack_x, 0) = x1;
	int y1 = slot( input->Y, 0);
	slot( stack_y, 0) = y1;
	int x2 = slot( input->X, 1);
	slot( stack_x, 1) = x2;
	int y2 = slot( input->Y, 1);
	slot( stack_y, 1) = y2;
	int ss = 2;
	int i = 2;
	int next_x = slot( input->X, i);
	int next_y = slot( input->Y, i);
	int prod = X_PROD(x1, y1, x2, y2, next_x, next_y);
	slot( stack_x, ss) = next_x;
	slot( stack_y, ss) = next_y;
	ss = ss + 1;
	x1 = x2;
	y1 = y2;
	x2 = next_x;
	y2 = next_y;
	i = i + 1;
	[[buffet::fsm(2*MAX_N)]]
	while(i < n){
		next_x = slot( input->X, i);
		next_y = slot( input->Y, i);
		prod = X_PROD(x1, y1, x2, y2, next_x, next_y);
		while(prod < 0){
			ss = ss - 1;
			x2 = x1;
			y2 = y1;
			x1 = slot( stack_x, ss-2);
			y1 = slot( stack_y, ss-2);
			prod = X_PROD(x1, y1, x2, y2, next_x, next_y);
		}
		slot( stack_x, ss) = next_x;
		slot( stack_y, ss) = next_y;
		ss = ss + 1;
		x1 = x2;
		y1 = y2;
		x2 = next_x;
		y2 = next_y;
		i = i + 1;
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->stack_y[ITER1] = stack_y[ITER1];
	}
	output->ss = ss;
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->stack_x[ITER1] = stack_x[ITER1];
	}
}
