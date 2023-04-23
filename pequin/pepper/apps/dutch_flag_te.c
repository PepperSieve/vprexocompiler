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
	int values[MAX_N + 1 + 1 + MAX_N + MAX_N + MAX_N];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[3] = {input->a_content, input->n, input->a_color};
	ghost_t ghost[1];
	int len[3] = {MAX_N, 1, MAX_N};
	exo_compute(public_info, len, ghost, 1);
	int ord_in_B[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		ord_in_B[ITER1] = ghost[0].values[0 + ITER1];
	}
	int k2 = ghost[0].values[0 + MAX_N];
	int k1 = ghost[0].values[0 + MAX_N + 1];
	int ord_in_A[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		ord_in_A[ITER1] = ghost[0].values[0 + MAX_N + 1 + 1 + ITER1];
	}
	int b_color[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		b_color[ITER1] = ghost[0].values[0 + MAX_N + 1 + 1 + MAX_N + ITER1];
	}
	int b_content[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		b_content[ITER1] = ghost[0].values[0 + MAX_N + 1 + 1 + MAX_N + MAX_N + ITER1];
	}
	int n = input->n[0];
	int accumErr = 0;
	int cur_color = 0;
	int count[MAX_N];
	int it4; for(it4 = 0; it4 < MAX_N; it4++) {
		slot(count, it4) = 0;
	}
	if(k1 < -1) { accumErr++; }
	if(k1 > k2) { accumErr++; }
	if(k2 >= n) { accumErr++; }
	int it5; for(it5 = 0; it5 < MAX_N; it5++) {
		int ord_j = slot(ord_in_A, it5);
		if(it5 - 1 == k1) {
			cur_color = cur_color + 1;
		}
		if(it5 - 1 == k2) {
			cur_color = cur_color + 1;
		}
		if(it5 < n) {
			if(ord_j >= n) { accumErr++; }
			slot(count, ord_j) = 1;
			if(slot( b_color, it5) != slot( input->a_color, ord_j)) { accumErr++; }
			if(slot( b_content, it5) != slot( input->a_content, ord_j)) { accumErr++; }
			int tmp_color = slot( b_color, it5);
			if(tmp_color - cur_color != 0) { accumErr++; }
		} else {
			slot(count, it5) = 1;
		}
	}
	int it6; for(it6 = 0; it6 < MAX_N; it6++) {
		if(slot(count, it6) - 1 != 0) { accumErr++; }
	}
	assert_zero(accumErr);
}
