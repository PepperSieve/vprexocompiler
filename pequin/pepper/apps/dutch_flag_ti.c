#define MAX_N 10
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct item {
  int color;
  int content;
};
struct In {
  struct item* a[MAX_N];
  int n;
};
struct Out {
  struct item* b[MAX_N];
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int n = input->n;
	int j = 0;
	int k = n - 1;
	int it1; for(it1 = 0; it1 < MAX_N; it1++){
		if(it1 < n) {
output->b[it1] = input->a[it1]; 
		}
	}
	int i = 0;
	int it2; for(it2 = 0; it2 < MAX_N; it2++){
		if(j <= k) {
struct item* bj = output->b[j]; 
int color = bj->color; 
			if(color == 0) {
struct item* tmp = output->b[i]; 
output->b[i] = bj; 
output->b[j] = tmp; 
				i = i + 1;
				j = j + 1;
			} else {
				if(color == 2) {
struct item* tmp = bj; 
output->b[j] = output->b[k]; 
output->b[k] = tmp; 
					k = k - 1;
				} else {
					j = j + 1;
				}
			}
		}
	}
}
