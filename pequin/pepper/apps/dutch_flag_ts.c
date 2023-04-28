#define MAX_N 20
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct item {
  int color;
  int content;
};
struct In {
  int a_content[MAX_N];
  int n[1];
  int a_color[MAX_N];
};
struct Out {
  int b_content[MAX_N];
  int b_color[MAX_N];
};
typedef struct ghost_s {
	int values[1 + 1 + MAX_N + MAX_N + MAX_N + MAX_N];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[3] = {input->a_content, input->n, input->a_color};
	ghost_t ghost[1];
	int len[3] = {MAX_N, 1, MAX_N};
	exo_compute(public_info, len, ghost, 1);
	int k2 = ghost[0].values[0];
	int k1 = ghost[0].values[0 + 1];
	int ord_in_B[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		ord_in_B[ITER1] = ghost[0].values[0 + 1 + 1 + ITER1];
	}
	int ord_in_A[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		ord_in_A[ITER1] = ghost[0].values[0 + 1 + 1 + MAX_N + ITER1];
	}
	int b_color[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		b_color[ITER1] = ghost[0].values[0 + 1 + 1 + MAX_N + MAX_N + ITER1];
	}
	int b_content[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		b_content[ITER1] = ghost[0].values[0 + 1 + 1 + MAX_N + MAX_N + MAX_N + ITER1];
	}
	int n = input->n[0];
	int accumErr = 0;
	int it7; for(it7 = 0; it7 < MAX_N; it7++){
		if(it7 < n) {
			int tmp_ord = slot( ord_in_A, it7);
			if(tmp_ord < 0) { accumErr++; }
			if(tmp_ord >= n) { accumErr++; }
			if(slot( b_color, it7) != slot( input->a_color, tmp_ord)) { accumErr++; }
			if(slot( b_content, it7) != slot( input->a_content, tmp_ord)) { accumErr++; }
			tmp_ord = slot( ord_in_B, it7);
			if(tmp_ord < 0) { accumErr++; }
			if(tmp_ord >= n) { accumErr++; }
			if(slot( input->a_color, it7) != slot( b_color, tmp_ord)) { accumErr++; }
			if(slot( input->a_content, it7) != slot( b_content, tmp_ord)) { accumErr++; }
		}
		int tmp_color = slot( b_color, it7);
		if(it7 <= k1) {
			if(tmp_color != 0) { accumErr++; }
		} else {
			if(it7 <= k2) {
				if(tmp_color - 1 != 0) { accumErr++; }
			} else {
				if(it7 < n) {
					if(tmp_color - 2 != 0) { accumErr++; }
				}
			}
		}
	}
	assert_zero(accumErr);
}
