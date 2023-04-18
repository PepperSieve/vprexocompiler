#define MAX_N 10
#define MAX_L 2
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int A[MAX_L * MAX_N];
  int N[MAX_L];
  int L[1];
};
struct Out {
  int B[MAX_L*MAX_N];
};
typedef struct ghost_s {
	int values[MAX_N*MAX_L + MAX_N*MAX_L + MAX_N*MAX_L];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[3] = {input->A, input->N, input->L};
	ghost_t ghost[1];
	int len[3] = {MAX_L * MAX_N, MAX_L, 1};
	exo_compute(public_info, len, ghost, 1);
	uint32_t J_i[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		J_i[ITER1] = ghost[0].values[0 + ITER1];
	}
	uint32_t K_i[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		K_i[ITER1] = ghost[0].values[0 + MAX_N*MAX_L + ITER1];
	}
	uint32_t B[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		B[ITER1] = ghost[0].values[0 + MAX_N*MAX_L + MAX_N*MAX_L + ITER1];
	}
	uint32_t L = input->L[0];
	uint32_t B_len_te = 0;
	int k4; for(k4 = 0; k4 < MAX_L; k4++) {
		if(k4 < L) {
			B_len_te = B_len_te + slot( input->N, k4);
		}
	}
	int k5; for(k5 = 0; k5 < MAX_L*MAX_N; k5++) {
		if(k5 < B_len_te) {
			assert_zero(k5 != 0 && slot( B, k5-1) >= slot( B, k5));
			uint32_t ki = slot(K_i, k5);
			assert_zero(ki >= L);
			uint32_t ji = slot(J_i, k5);
			uint32_t ni = slot( input->N, ki);
			assert_zero(ji >= ni);
			uint32_t bi = slot( B, k5);
			assert_zero(bi != mat_slot( input->A, MAX_N, ki, ji ));
		}
	}
}
