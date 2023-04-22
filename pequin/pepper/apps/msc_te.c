#define MAX_V 5
#define MAX_E 10
#define MAX_EXP 32
#include <stdint.h>
#define slot(A, i) A[i]
#define mat_slot(A, n, i, j) A[i * n + j]
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
	int level_s[MAX_V];
	int head_s[MAX_V];
	int next_s[MAX_V];
	int remn_s[MAX_V];
	int sp = 0;
	int level = 0;
	int occ_te[MAX_V];
	int k3; for(k3 = 0; k3 < MAX_V; k3++) {
		slot( occ_te, k3) = 0;
	}
	int head = -1;
	int next = -1;
	int remn = -1;
	int cur_msc_te = -1;
	int k4; for(k4 = 0; k4 < 2*MAX_V-1; k4++) {
		if(slot(T, k4) < NV) {
			if(level == 0) {
				cur_msc_te = cur_msc_te + 1;
			}
			int v_recv = -1;
			int v_outg = -1;
			int remn_last = -1;
			if(slot(T, k4) < 0) {
				remn_last = remn - slot(P_remn, k4) - 1;
				remn = slot(P_remn, k4);
				int recv = slot(P_recv, k4);
				int outg = slot(P_outg, k4);
				if(recv <= k4) { accumErr++; }
				if(outg <= k4) { accumErr++; }
				if(recv > k4 + remn) { accumErr++; }
				if(outg > k4 + remn) { accumErr++; }
				v_recv = slot(T, recv);
				v_outg = slot(T, outg);
			} else {
				int v = slot(T, k4);
				if(slot( output->MSC, v) - cur_msc_te != 0) { accumErr++; }
				slot(occ_te, v) = 1;
				v_recv = v;
				v_outg = v;
				remn = remn - 1;
			}
			if(level != 0) {
				if(head == -1) {
					head = v_recv;
				}
				if(next != -1) {
					if(v_recv - next != 0) { accumErr++; }
				}
				if(slot(next_T, k4) == NV) {
					next = head;
					if(remn_last != 0) { accumErr++; }
				} else {
					next = slot(next_T, k4);
				}
				int e_te = slot(E, k4);
				if(e_te < 0) { accumErr++; }
				if(e_te >= NE) { accumErr++; }
				if(slot( input->edgeV, e_te) - v_outg != 0) { accumErr++; }
				if(slot( input->edges, e_te) - next != 0) { accumErr++; }
			}
			if(slot(T, k4) < 0) {
				if(remn_last != 0) {
					slot( level_s, sp) = level;
					slot( head_s, sp) = head;
					slot( next_s, sp) = next;
					slot( remn_s, sp) = remn_last;
					sp = sp + 1;
				}
				level = level + 1;
				head = -1;
				next = -1;
			}
			if(level != 0 && remn == 0) {
				sp = sp - 1;
				level = slot( level_s, sp);
				head = slot( head_s, sp);
				next = slot( next_s, sp);
				remn = slot( remn_s, sp);
			}
		}
	}
	if(level != 0) { accumErr++; }
	int count_te = 0;
	int k5; for(k5 = 0; k5 < MAX_V; k5++) {
		if(k5 < NV && slot(occ_te, k5) == 1) {
			count_te = count_te + 1;
		}
	}
	if(count_te != 0) { accumErr++; }
	output->MSCnum = cur_msc_te + 1;
	cur_msc_te = slot( output->MSC, 0);
	int i_te = 0;
	int j_te = 0;
	int ebi0_te = slot( input->edgeB, 0);
	int ebi1_te = slot( input->edgeB, 1);
	int k6; for(k6 = 0; k6 < MAX_E+MAX_V; k6++) {
		if(i_te < NV) {
			if(j_te == ebi1_te) {
				i_te = i_te + 1;
				cur_msc_te = slot( output->MSC, i_te);
				ebi0_te = ebi1_te;
				if(i_te < NV) {
					ebi1_te = slot( input->edgeB, i_te+1);
				}
			} else {
				if(slot( output->MSC, slot( input->edges, j_te) ) > cur_msc_te) { accumErr++; }
				j_te = j_te + 1;
			}
		}
	}
	assert_zero(accumErr);
}
