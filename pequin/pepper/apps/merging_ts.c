#define MAX_N 10
#define MAX_L 4
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
	int values[MAX_N*MAX_L * MAX_N*MAX_L + MAX_N*MAX_L + MAX_N*MAX_L + MAX_N*MAX_L];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[3] = {input->A, input->L, input->N};
	ghost_t ghost[1];
	int len[3] = {MAX_L * MAX_N, 1, MAX_L};
	exo_compute(public_info, len, ghost, 1);
	uint32_t IND[MAX_N*MAX_L * MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L * MAX_N*MAX_L; ITER1++) {
		IND[ITER1] = ghost[0].values[0 + ITER1];
	}
	uint32_t J_i[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		J_i[ITER1] = ghost[0].values[0 + MAX_N*MAX_L * MAX_N*MAX_L + ITER1];
	}
	uint32_t B[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		B[ITER1] = ghost[0].values[0 + MAX_N*MAX_L * MAX_N*MAX_L + MAX_N*MAX_L + ITER1];
	}
	uint32_t K_i[MAX_N*MAX_L];
	for (ITER1 = 0; ITER1 < MAX_N*MAX_L; ITER1++) {
		K_i[ITER1] = ghost[0].values[0 + MAX_N*MAX_L * MAX_N*MAX_L + MAX_N*MAX_L + MAX_N*MAX_L + ITER1];
	}
	uint32_t L = input->L[0];
	int accumErr = 0;
	uint32_t B_len_ts = 0;
	uint32_t k6; for(k6 = 0; k6 < MAX_L; k6++){
		if(k6 < L) {
			B_len_ts = B_len_ts + slot( input->N, k6);
		}
	}
	uint32_t k7; for(k7 = 0; k7 < MAX_L*MAX_N; k7++){
		if(k7 < B_len_ts) {
			if(k7 != 0 && slot( B, k7-1) >= slot( B, k7)) { accumErr++; }
			uint32_t ki = slot(K_i, k7);
			if(ki >= L) { accumErr++; }
			uint32_t ji = slot(J_i, k7);
			uint32_t ni = slot( input->N, ki);
			if(ji >= ni) { accumErr++; }
			uint32_t bi = slot( B, k7);
			if(bi != mat_slot( input->A, MAX_N, ki, ji )) { accumErr++; }
		}
	}
	uint32_t k8; for(k8 = 0; k8 < MAX_L; k8++){
		if(k8 < L) {
			uint32_t k9; for(k9 = 0; k9 < MAX_N; k9++){
				if(k9 < slot( input->N, k8)) {
					uint32_t ind = mat_slot( IND, MAX_N, k8, k9 );
					if(0 > ind) { accumErr++; }
					if(ind >= B_len_ts) { accumErr++; }
					if(mat_slot( input->A, MAX_N, k8, k9 ) != slot( B, ind)) { accumErr++; }
				}
			}
		}
	}
	assert_zero(accumErr);
}
