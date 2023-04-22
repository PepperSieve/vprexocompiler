#define MAX_N 20
#define MAX_M 4
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct In {
  int PAT[MAX_M];
  int M;
  int TXT[MAX_N];
  int N;
};
struct Out {
  int ind;
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int M = input->M;
	int N = input->N;
	int lps[MAX_M];
	int l = 0;
	slot(lps, 0) = 0;
	int i = 1;
	int ind = 0;
	int k1; for(k1 = 0; k1 < MAX_M*2; k1++){
		if(i < M) {
			if(slot( input->PAT, i) == slot( input->PAT, l)) {
				l = l + 1;
				slot(lps, i) = l;
				i = i + 1;
			} else {
				if(l != 0) {
					l = slot(lps, l-1);
				} else {
					slot( lps, i) = 0;
					i = i + 1;
				}
			}
		}
	}
	i = 0;
	int j = 0;
	int found = 0;
	int k2; for(k2 = 0; k2 < MAX_N*2; k2++){
		if(i < N) {
			if(j < M) {
				if(slot( input->PAT, j) != slot( input->TXT, i)) {
					if(j != 0) {
						j = slot(lps, j-1);
					} else {
						i = i + 1;
					}
				} else {
					j = j + 1;
					i = i + 1;
				}
			}
			if(j == M) {
				ind = i - M;
				found = 1;
				i = N + 1;
				};
			}
		}
		if(found == 0) {
			ind = N;
		}
	output->ind = ind;
}
