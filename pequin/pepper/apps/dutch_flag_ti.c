#define MAX_N 10
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct item {
  int color;
  int content;
};
struct In {
  int a_content[MAX_N];
  int n;
  int a_color[MAX_N];
};
struct Out {
  int b_content[MAX_N];
  int b_color[MAX_N];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int n = input->n;
	int j = 0;
	int k = n - 1;
	int b_color[MAX_N];
	int b_content[MAX_N];
	int it1; for(it1 = 0; it1 < MAX_N; it1++){
		if(it1 < n) {
			slot( b_color, it1) = slot( input->a_color, it1);
			slot( b_content, it1) = slot( input->a_content, it1);
		}
	}
	int i = 0;
	int it2; for(it2 = 0; it2 < MAX_N; it2++){
		if(j <= k) {
			int bj_color = slot( b_color, j);
			int bj_content = slot( b_content, j);
			if(bj_color == 0) {
				int tmp_color = slot( b_color, i);
				int tmp_content = slot( b_content, i);
				slot( b_color, i) = bj_color;
				slot( b_content, i) = bj_content;
				slot( b_color, j) = tmp_color;
				slot( b_content, j) = tmp_content;
				i = i + 1;
				j = j + 1;
			} else {
				if(bj_color == 2) {
					slot( b_color, j) = slot( b_color, k);
					slot( b_content, j) = slot( b_content, k);
					slot( b_color, k) = bj_color;
					slot( b_content, k) = bj_content;
					k = k - 1;
				} else {
					j = j + 1;
				}
			}
		}
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->b_content[ITER1] = b_content[ITER1];
	}
	for(ITER1 = 0; ITER1 < MAX_N; ITER1++) {
		output->b_color[ITER1] = b_color[ITER1];
	}
}
