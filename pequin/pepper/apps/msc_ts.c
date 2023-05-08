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
int chain_to_T(struct T_struct* T_ptr, int T_sp, int comps[MAX_V * MAX_V], int comps_sp, int comps_ind[MAX_V], int cur_comp) {
    int T_head = T_sp;
    T_ptr[T_head].val = -1;
    T_sp++;
    int i;
    for (i = 0; i < comps_ind[cur_comp]; i++) {
        int target = comps[cur_comp * MAX_V + i];
        if (target >= 0) {
            T_ptr[T_head].val = target;
            T_sp++;
        } else {
            T_sp = chain_to_T(T_ptr, T_sp, comps, comps_sp, comps_ind, -1 * target);
        }
    }
    T_ptr[T_head].remn = T_sp - T_head - 1;
    return T_sp;
}
struct In {
  int edges[MAX_E];
  int edgeB[MAX_V+1];
  int NV[1];
  int edgeV[MAX_E];
  int NE[1];
};
struct Out {
  int MSC[MAX_V];
  int MSCnum;
};
typedef struct ghost_s {
	int values[MAX_EXP + 2*MAX_V-1 + MAX_V + MAX_EXP + MAX_EXP + 2*MAX_V-1 + 1 + 2*MAX_V-1 + 2*MAX_V-1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[5] = {input->edges, input->edgeB, input->NV, input->edgeV, input->NE};
	ghost_t ghost[1];
	int len[5] = {MAX_E, MAX_V+1, 1, MAX_E, 1};
	exo_compute(public_info, len, ghost, 1);
	int cycl[MAX_EXP];
	for (ITER1 = 0; ITER1 < MAX_EXP; ITER1++) {
		cycl[ITER1] = ghost[0].values[0 + ITER1];
	}
	int next_T[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		next_T[ITER1] = ghost[0].values[0 + MAX_EXP + ITER1];
	}
	int MSC[MAX_V];
	for (ITER1 = 0; ITER1 < MAX_V; ITER1++) {
		MSC[ITER1] = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + ITER1];
	}
	int T[MAX_EXP];
	for (ITER1 = 0; ITER1 < MAX_EXP; ITER1++) {
		T[ITER1] = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + MAX_V + ITER1];
	}
	int E[MAX_EXP];
	for (ITER1 = 0; ITER1 < MAX_EXP; ITER1++) {
		E[ITER1] = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + MAX_V + MAX_EXP + ITER1];
	}
	int P_remn[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		P_remn[ITER1] = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + MAX_V + MAX_EXP + MAX_EXP + ITER1];
	}
	int MSCnum = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + MAX_V + MAX_EXP + MAX_EXP + 2*MAX_V-1];
	int P_recv[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		P_recv[ITER1] = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + MAX_V + MAX_EXP + MAX_EXP + 2*MAX_V-1 + 1 + ITER1];
	}
	int P_outg[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		P_outg[ITER1] = ghost[0].values[0 + MAX_EXP + 2*MAX_V-1 + MAX_V + MAX_EXP + MAX_EXP + 2*MAX_V-1 + 1 + 2*MAX_V-1 + ITER1];
	}
	int NV = input->NV[0];
	int NE = input->NE[0];
	int accumErr = 0;
	int occ_ts[MAX_V];
	int k7; for(k7 = 0; k7 < MAX_V; k7++){
		slot( occ_ts, k7) = 0;
	}
	int cur_msc_ts = 0;
	int cur_head = slot( T, 0);
	if(slot( MSC, 0) != 0) { accumErr++; }
	if(slot( input->edgeV, 0) != 0) { accumErr++; }
	int e_ts = slot( E, 0);
	if(e_ts < 0) { accumErr++; }
	if(e_ts >= NE) { accumErr++; }
	if(slot( input->edgeV, e_ts) - cur_head != 0) { accumErr++; }
	int k8; for(k8 = 0; k8 < MAX_EXP-1; k8++){
		if(cur_msc_ts < MSCnum) {
			int v = slot( T, k8+1);
			slot( occ_ts, v) = 1;
			if(slot( cycl, k8+1) == 0) {
				cur_msc_ts = cur_msc_ts + 1;
				if(slot( input->edges, e_ts) - cur_head != 0) { accumErr++; }
				cur_head = v;
			} else {
				if(slot( input->edges, e_ts) - v != 0) { accumErr++; }
			}
			e_ts = slot( E, v);
			if(e_ts < 0) { accumErr++; }
			if(e_ts >= NE) { accumErr++; }
			if(cur_msc_ts != MSCnum) {
				if(slot( input->edgeV, e_ts) - v != 0) { accumErr++; }
			}
		}
	}
	int count_ts = 0;
	int k9; for(k9 = 0; k9 < MAX_V; k9++){
		if(k9 < NV && slot(occ_ts, k9) == 1) {
			count_ts = count_ts + 1;
		}
	}
	if(count_ts != 0) { accumErr++; }
	MSCnum = cur_msc_ts + 1;
	cur_msc_ts = slot( MSC, 0);
	int i_ts = 0;
	int j_ts = 0;
	int ebi0_ts = slot( input->edgeB, 0);
	int ebi1_ts = slot( input->edgeB, 1);
	int k10; for(k10 = 0; k10 < MAX_E+MAX_V; k10++){
		if(i_ts < NV) {
			if(j_ts == ebi1_ts) {
				i_ts = i_ts + 1;
				cur_msc_ts = slot( MSC, i_ts);
				ebi0_ts = ebi1_ts;
				if(i_ts < NV) {
					ebi1_ts = slot( input->edgeB, i_ts+1);
				}
			} else {
				if(slot( MSC, slot( input->edges, j_ts) ) > cur_msc_ts) { accumErr++; }
				j_ts = j_ts + 1;
			}
		}
	}
	assert_zero(accumErr);
}
