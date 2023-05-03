#define MAX_N 320
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct item {
  int color;
  int content;
};
struct In {
  struct item* a[MAX_N];
  int n[1];
};
struct Out {
  struct item* b[MAX_N];
};
typedef struct ghost_s {
	int values[MAX_N + 1 + 1 + MAX_N + MAX_N];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[1] = {input->n};
	ghost_t ghost[1];
	int len[1] = {1};
	exo_compute(public_info, len, ghost, 1);
	int ord[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		ord[ITER1] = ghost[0].values[0 + ITER1];
	}
	int k2 = ghost[0].values[0 + MAX_N];
	int k1 = ghost[0].values[0 + MAX_N + 1];
	int bj_color[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		bj_color[ITER1] = ghost[0].values[0 + MAX_N + 1 + 1 + ITER1];
	}
	int bj_content[MAX_N];
	for (ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		bj_content[ITER1] = ghost[0].values[0 + MAX_N + 1 + 1 + MAX_N + ITER1];
	}
	int n = input->n[0];
	int cur_color = 0;
struct item* b[MAX_N]; 
	int count[MAX_N];
	int it4; for(it4 = 0; it4 < MAX_N; it4++) {
		slot(count, it4) = 0;
b[it4]->color = bj_color[it4]; 
b[it4]->content = bj_content[it4]; 
	}
	assert_zero(k1 < -1);
	assert_zero(k1 > k2);
	assert_zero(k2 >= n);
	int it5; for(it5 = 0; it5 < MAX_N; it5++) {
		int ord_j = slot(ord, it5);
		if(it5 - 1 == k1) {
			cur_color = cur_color + 1;
		}
		if(it5 - 1 == k2) {
			cur_color = cur_color + 1;
		}
		if(it5 < n) {
			assert_zero(ord_j >= n);
			slot(count, ord_j) = 1;
assert_zero(b[it5] != input->a[ord_j]); 
			int tmp_color = slot(bj_color, it5);
			assert_zero(tmp_color - cur_color);
		} else {
			slot(count, it5) = 1;
		}
	}
	int it6; for(it6 = 0; it6 < MAX_N; it6++) {
		assert_zero(slot(count, it6) - 1);
	}
}
