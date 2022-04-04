// The goal of this program is that given positive integers k >= 2 and r, 
// find positive integers x and y such that x^k + y^k == r, x >= y.
// Since there might be more than one (x, y) pair, we store the result in arrays. However, I believe that
// we cannot get more than 2 pairs of (x, y).
// We also assume that we cannot use the square root operation.

#include <stdint.h>

#define MAX_OUT 2

struct In {
	uint32_t r;
	uint32_t k;
};

struct Out {
	uint32_t x[MAX_OUT];
	uint32_t y[MAX_OUT];
};

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

void compute(struct In *input, struct Out *output) {
	int x = 0;
	int y = 0;
	int r = input->r;
	int k = input->k;
	int tmp;

	for (tmp = 0; tmp < MAX_R; tmp++) {
		if (pow(x, k) + pow(y, k) < r) {
			x++;
			y++;
		}
	}
	int i = 0;
	for (i = 0; i < MAX_OUT; i++) {
		output->x[i] = -1;
		output->y[i] = -1;
	}
	i = 0;
	for (tmp = 0; tmp < 2 * (MAX_R + 1); tmp++) {
		if (pow(x, k) <= r) {
			if (pow(x, k) + pow(y, k) > r) y--;
			else if (pow(x, k) + pow(y, k) == r) {
				(output->x)[i] = x;
				(output->y)[i] = y;
				i++;
				x++;
			} else if (pow(x, k) + pow(y, k) < r) {
				x++;
			}
		}
	}
}
