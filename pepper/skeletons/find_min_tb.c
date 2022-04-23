#include <stdint.h>

struct In {
	uint32_t n;
	uint32_t a[MAX_N];
	uint32_t b[MAX_N];
	uint32_t min;
};

struct Out {
	uint32_t min;
};

void compute(struct In *input, struct Out *output) {
	uint32_t min = input->min;
	int n = input->n;
	int k, count;

	for (k = 0; k < MAX_N; k++) {
		if (k < n) {
			assert_zero(min > input->a[k]);
			if (min == input->a[k]) {
				count++;
				assert_zero(input->b[k] - 1);
			}
			else assert_zero(input->b[k]);
		}
	}

	assert_zero(count == 0);
}
