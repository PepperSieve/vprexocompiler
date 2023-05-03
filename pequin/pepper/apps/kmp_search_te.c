#define MAX_N 320
#define MAX_M 128
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
void set_mat(int* LM, int i, int j, int k, int M) {
    int hit;
    for (hit = i; hit < M; hit++) {
        LM[hit * MAX_N + hit - i + j] = k;
    }
}
void copy_lps_mat(int* LM, int i1, int i2, int j1, int j2, int M) {
    if (j1 == j2) {
        set_mat(LM, i2, j1, LM[i1 * MAX_N + j1], M);
    } else {
        copy_lps_mat(LM, i1, i2, j1 + 1, j2, M);
        set_mat(LM, i2, j1, LM[i1 * MAX_N + j1], M);
    }
}
int update_b(int* LM, int* B, int B_ind, int j, int lpsj) {
    if (lpsj == j) {
        B[B_ind] = LM[j * MAX_N + lpsj];
        return B_ind + 1;
    } else {
        update_b(LM, B, B_ind, j, lpsj + 1);
        B[B_ind] = LM[j * MAX_N + lpsj];
        return B_ind + 1;
    }
}
struct In {
  int PAT[MAX_M];
  int M[1];
  int TXT[MAX_N];
  int N[1];
};
struct Out {
  int ind;
};
typedef struct ghost_s {
	int values[MAX_N + 1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[4] = {input->PAT, input->M, input->TXT, input->N};
	ghost_t ghost[1];
	int len[4] = {MAX_M, 1, MAX_N, 1};
	exo_compute(public_info, len, ghost, 1);
	int B[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		B[ITER1] = ghost[0].values[0 + ITER1];
	}
	int ind = ghost[0].values[0 + MAX_N];
	int M = input->M[0];
	int N = input->N[0];
	assert_zero(ind > N - M + 1);
	int k3; for(k3 = 0; k3 < MAX_N; k3++) {
		if(k3 < ind) {
			int b_i = slot(B, k3);
			assert_zero(b_i >= M);
			int t = slot( input->TXT, k3+b_i);
			int p = slot( input->PAT, b_i);
			assert_zero(t == p);
		}
	}
	if(ind != N - M + 1) {
		int k4; for(k4 = 0; k4 < MAX_M; k4++) {
			if(k4 < M) {
				int t = slot( input->TXT, ind+k4);
				int p = slot( input->PAT, k4);
				assert_zero(t != p);
			}
		}
	} else {
		ind = N;
	}
}
