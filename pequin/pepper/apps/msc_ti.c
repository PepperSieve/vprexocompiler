#define MAX_V 5
#define MAX_E 10
#define MAX_EXP 32
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
struct T_struct {
    int val;
    int remn;
    int outg;
    int recv;
    int next;
    int edge;
};
// Append everything in cur_comp to T
int chain_to_T(struct T_struct* T_ptr, int T_sp, int comps[MAX_V * MAX_V], int comps_E[MAX_V * MAX_V], int comps_outg[MAX_V], int comps_sp, int comps_ind[MAX_V], int cur_comp) {
    int T_head = T_sp;
    T_ptr[T_head].val = -1;
    T_sp++;
    int i;
    for (i = 0; i < comps_ind[cur_comp]; i++) {
        int target = comps[cur_comp * MAX_V + i];
        int next = i == comps_ind[cur_comp] - 1 ? comps[cur_comp * MAX_V] : comps[cur_comp * MAX_V + i + 1];
        if (target >= 0) {
            T_ptr[T_sp].val = target;
            T_ptr[T_sp].edge = comps_E[cur_comp * MAX_V + i];
            T_ptr[T_sp].next = next;
            T_sp++;
        } else {
            T_sp = chain_to_T(T_ptr, T_sp, comps, comps_E, comps_outg, comps_sp, comps_ind, -1 * target);
        }
    }
    T_ptr[T_head].remn = T_sp - T_head - 1;
    T_ptr[T_head].recv = T_head + 1;
    T_ptr[T_head].outg = T_head + comps_outg[cur_comp] + 1;
    return T_sp;
}
struct In {
  int edges[MAX_E];
  int edgeB[MAX_V+1];
  int NV;
  int edgeV[MAX_E];
  int NE;
};
struct Out {
  int MSC[MAX_V];
  int MSCnum;
};
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int NV = input->NV;
	int NE = input->NE;
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
		slot( cc, cc_sp) = 1;
		cc_sp = cc_sp + 1;
		slot( tvb, tvb_sp) = 0;
		tvb_sp = tvb_sp + 1;
		int j = slot( input->edgeB, v+1);
		while(j > slot( input->edgeB, v)){
			j = j - 1;
			slot( tv, tv_sp) = slot( input->edges, j);
			tv_sp = tv_sp + 1;
		}
		while(cc_sp > 0){
			while(tv_sp > slot(tvb, tvb_sp-1)){
				// pop(tv);
				tv_sp = tv_sp - 1;
				v = slot( tv, tv_sp);
				while(slot(rank, v) > 0 && slot(cc, cc_sp-1) > slot(rank, v)){
					cc_sp = cc_sp - 1;
					tvb_sp = tvb_sp - 1;
				}
				if(slot(rank, v) == 0) {
					in_inner_loop = 1;
					nvc = nvc + 1;
					slot(rank, v) = nvc;
					slot(knar, nvc) = v;
					slot( cc, cc_sp) = nvc;
					cc_sp = cc_sp + 1;
					slot( tvb, tvb_sp) = tv_sp;
					tvb_sp = tvb_sp + 1;
					j = slot( input->edgeB, v+1);
				}
				while(in_inner_loop == 1 && j > slot( input->edgeB, v)){
					j = j - 1;
					slot( tv, tv_sp) = slot( input->edges, j);
					tv_sp = tv_sp + 1;
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
