#define MAX_N 10
#define MAX_L 14
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_L * MAX_N];
  int L[1];
  int N[MAX_L];
};
struct Out {
  int B[MAX_L*MAX_N];
};
typedef struct ghost_s {
	int values[MAX_N*MAX_L + MAX_N*MAX_L + MAX_N*MAX_L];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[3] = {input->A, input->L, input->N};
	ghost_t ghost[1];
	int len[3] = {MAX_L * MAX_N, 1, MAX_L};
	exo_compute(public_info, len, ghost, 1);
	uint32_t J_i[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		J_i[ITER1] = ghost[0].values[0 + ITER1];
	}
	uint32_t B[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		B[ITER1] = ghost[0].values[0 + MAX_N*MAX_L + ITER1];
	}
	uint32_t K_i[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		K_i[ITER1] = ghost[0].values[0 + MAX_N*MAX_L + MAX_N*MAX_L + ITER1];
	}
	uint32_t L = input->L[0];
	int accumErr = 0;
	assert_zero(accumErr);
}
