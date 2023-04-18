#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
};
struct Out {
};
typedef struct ghost_s {
	int values[];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[0] = {};
	ghost_t ghost[1];
	int len[0] = {};
	exo_compute(public_info, len, ghost, 1);
}
