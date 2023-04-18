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
	int values[2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1];
} ghost_t;
void compute(struct In *input, struct Out *output) {
	int ITER1; int ITER2;
	int *public_info[5] = {input->edges, input->edgeB, input->NV, input->edgeV, input->NE};
	ghost_t ghost[1];
	int len[5] = {MAX_E, MAX_V+1, 1, MAX_E, 1};
	exo_compute(public_info, len, ghost, 1);
	int next_T[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		next_T[ITER1] = ghost[0].values[0 + ITER1];
	}
	int P_outg[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		P_outg[ITER1] = ghost[0].values[0 + 2*MAX_V-1 + ITER1];
	}
	int E[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		E[ITER1] = ghost[0].values[0 + 2*MAX_V-1 + 2*MAX_V-1 + ITER1];
	}
	int P_recv[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		P_recv[ITER1] = ghost[0].values[0 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + ITER1];
	}
	int T[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		T[ITER1] = ghost[0].values[0 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + ITER1];
	}
	int P_remn[2*MAX_V-1];
	for (ITER1 = 0; ITER1 < 2*MAX_V-1; ITER1++) {
		P_remn[ITER1] = ghost[0].values[0 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + 2*MAX_V-1 + ITER1];
	}
	int NV = input->NV[0];
	int NE = input->NE[0];
	int level_s[MAX_V];
	int head_s[MAX_V];
	int next_s[MAX_V];
	int remn_s[MAX_V];
	int sp = 0;
	int level = 0;
	int occ[MAX_V];
	int k3; for(k3 = 0; k3 < MAX_V; k3++) {
		occ[k3] = 0;
	}
	int head = -1;
	int next = -1;
	int remn = -1;
	int cur_msc = -1;
	int k4; for(k4 = 0; k4 < 2*MAX_V-1; k4++) {
		if(slot(T, k4) < NV) {
			if(level == 0) {
				cur_msc = cur_msc + 1;
			}
			int v_recv = -1;
			int v_outg = -1;
			int remn_last = -1;
			if(slot(T, k4) < 0) {
				remn_last = remn - slot(P_remn, k4) - 1;
				remn = slot(P_remn, k4);
				int recv = slot(P_recv, k4);
				int outg = slot(P_outg, k4);
				assert_zero(recv <= k4);
				assert_zero(outg <= k4);
				assert_zero(recv > k4 + remn);
				assert_zero(outg > k4 + remn);
				v_recv = slot(T, recv);
				v_outg = slot(T, outg);
			} else {
				int v = slot(T, k4);
				assert_zero(slot( output->MSC, v) - cur_msc);
				slot(occ, v) = 1;
				v_recv = v;
				v_outg = v;
				remn = remn - 1;
			}
			if(level != 0) {
				if(head == -1) {
					head = v_recv;
				}
				if(next != -1) {
					assert_zero(v_recv - next);
				}
				if(slot(next_T, k4) == NV) {
					next = head;
					assert_zero(remn_last);
				} else {
					next = slot(next_T, k4);
				}
				int e = slot(E, k4);
				assert_zero(e < 0);
				assert_zero(e >= NE);
				assert_zero(slot( input->edgeV, e) - v_outg);
				assert_zero(slot( input->edges, e) - next);
			}
			if(slot(T, k4) < 0) {
				if(remn_last != 0) {
					stack_push(level, head, next, remn_last);
				}
				level = level + 1;
				head = -1;
				next = -1;
			}
			if(level != 0 && remn == 0) {
				stack_pop(level, head, next, remn);
			}
		}
	}
	assert_zero(level);
	int count = 0;
	int k5; for(k5 = 0; k5 < MAX_V; k5++) {
		if(k5 < NV && slot(occ, k5) == 1) {
			count = count + 1;
		}
	}
	assert_zero(count);
	output->MSCnum = cur_msc + 1;
	cur_msc = slot( output->MSC, 0);
	int i = 0;
	int j = 0;
	int ebi0 = slot( input->edgeB, 0);
	int ebi1 = slot( input->edgeB, 1);
	int k6; for(k6 = 0; k6 < MAX_E+MAX_V; k6++) {
		if(i < NV) {
			if(j == ebi1) {
				i = i + 1;
				cur_msc = slot( output->MSC, i);
				ebi0 = ebi1;
				if(i < NV) {
					ebi1 = slot( input->edgeB, i+1);
				}
			} else {
				assert_zero(slot( output->MSC, slot( input->edges, j) ) > cur_msc);
				j = j + 1;
			}
		}
	}
}
