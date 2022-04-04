#include <stdint.h>

struct In {
	// both l and r are inclusive
	uint32_t l;
	uint32_t r;
	uint32_t x;
	uint32_t a[MAX_N];
};

struct Out {
	uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
	uint32_t l = input->l;
	uint32_t r = input->r;
	uint32_t x = input->x;
	uint32_t found = 0;
	uint32_t amid, tmp;
	for (tmp = 0; tmp < MAX_LOG; tmp++) {
		if (r >= l) {
			uint32_t mid = l + (r - l) / 2;
			amid = (input->a)[mid];
			if (amid == x) {
				output->ind = mid;
				found = 1;
				r = l + 1;
			}
			if (amid > x)
				r = mid - 1;
			else
				l = mid + 1;
		}
	}
	if (found == 0) output->ind = -1;
}
