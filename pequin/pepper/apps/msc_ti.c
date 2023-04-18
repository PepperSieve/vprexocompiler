#define MAX_V 5
#define MAX_E 10
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
#define tv_push(x) tv[tv_sp] = x; tv_sp++;
#define tv_pop(x)  tv_sp--; x = tv[tv_sp];
#define cc_push(x) cc[cc_sp] = x; cc_sp++;
#define cc_pop(x)  cc_sp--; x = cc[cc_sp];
#define tvb_push(x) tvb[tvb_sp] = x; tvb_sp++;
#define tvb_pop(x) tvb_sp--; x = tvb[tvb_sp];
#define stack_push(level, head, next, remn) level_s[sp] = level; head_s[sp] = head; next_s[sp] = next; remn_s[sp] = remn; sp++;
#define stack_pop(level, head, next, remn) sp--; level = level_s[sp]; head = head_s[sp]; next = next_s[sp]; remn = remn_s[sp];
struct In {
  int edges[MAX_E];
  int edgeV[MAX_E];
  int edgeB[MAX_V+1];
  int NE;
  int NV;
};
struct Out {
  int MSC[MAX_V];
  int MSCnum;
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int NE = input->NE;
	int NV = input->NV;
	int MSCnum = -1;
	int rank[MAX_V];
	int cc[MAX_V];
	int cc_sp = 0;
	int k1; for(k1 = 0; k1 < MAX_V; k1++){
		slot(rank, k1) = 0;
	}
	int knar[MAX_V+1];
	int tv[MAX_V];
	int tv_sp = 0;
	int tvb[MAX_V];
	int tvb_sp = 0;
	int sv1count = 0;
	int cand = 0;
	int in_inner_loop = 0;
	[[buffet::fsm(2*MAX_V+MAX_E)]]
	while(sv1count < NV){
		while(slot(rank, cand) != 0){
			cand = cand + 1;
		}
		int v = cand;
		int nvc = 1;
		slot(rank, v) = 1;
		slot(knar, 1) = v;
		cc_push(1);
		tvb_push(0);
		int j = slot( input->edgeB, v+1);
		while(j > slot( input->edgeB, v)){
			j = j - 1;
			tv_push( slot( input->edges, j) );
		}
		while(cc_sp > 0){
			while(tv_sp > slot(tvb, tvb_sp-1)){
				tv_pop(v);
				while(slot(rank, v) > 0 && slot(cc, cc_sp-1) > slot(rank, v)){
					cc_sp = cc_sp - 1;
					tvb_sp = tvb_sp - 1;
				}
				if(slot(rank, v) == 0) {
					in_inner_loop = 1;
					nvc = nvc + 1;
					slot(rank, v) = nvc;
					slot(knar, nvc) = v;
					cc_push(nvc);
					tvb_push(tv_sp);
					j = slot( input->edgeB, v+1);
				}
				while(in_inner_loop == 1 && j > slot( input->edgeB, v)){
					j = j - 1;
					tv_push( slot( input->edges, j) );
				}
				in_inner_loop = 0;
			}
			MSCnum = MSCnum + 1;
			int tmp = slot(cc, cc_sp-1);
			while(nvc >= tmp){
				nvc = nvc - 1;
				slot( rank, slot( knar, nvc+1) ) = NV + MSCnum;
				sv1count = sv1count + 1;
			}
			cc_sp = cc_sp - 1;
			tvb_sp = tvb_sp - 1;
		}
	}
	MSCnum = MSCnum + 1;
	int MSC[MAX_V];
	int k2; for(k2 = 0; k2 < MAX_V; k2++){
		if(k2 < NV) {
			slot(MSC, k2) = slot(rank, k2) - NV;
		}
	}
	for(ITER1 = 0; ITER1 < MAX_V; ITER1++) {
		output->MSC[ITER1] = MSC[ITER1];
	}
	output->MSCnum = MSCnum;
}
