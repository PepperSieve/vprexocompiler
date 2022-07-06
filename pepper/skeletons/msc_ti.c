#include <stdint.h>

#define tv_push(x) tv[tv_sp] = x; tv_sp++;
#define tv_pop(x)  tv_sp--; x = tv[tv_sp];

#define cc_push(x) cc[cc_sp] = x; cc_sp++;
#define cc_pop(x)  cc_sp--; x = cc[cc_sp];

#define tvb_push(x) tvb[tvb_sp] = x; tvb_sp++;
#define tvb_pop(x) tvb_sp--; x = tvb[tvb_sp];

struct In {
    uint32_t NV;
    // Record down the number of edges in the extra slot of edgeB
    uint32_t edgeB[MAX_V + 1];
    uint32_t edges[MAX_E];
};

struct Out {
    uint32_t MSCnum;
    uint32_t MSC[MAX_V];
};

void compute(struct In *input, struct Out *output) {
    int NV = input->NV;

    int i, j, k;
    // initialize MSC
    output->MSCnum = -1;

    // rank[] is equivalent to INDEX in tarjan, used to keep track
    // of whether a vertex is processed
    int rank[MAX_V];
    // cc[cc_sp - 1] is used to denote the rank of the oldest vertex in chain
    int cc[MAX_V];
    int cc_sp = 0;
    for (i = 0; i < MAX_V; i++) rank[i] = 0;
    // # of vertices in sv2
    // 1 <= rank[v] <= nvc: v in sv2
    // rank[v] >= NV + 1: v in sv1
    int nvc;
    // knar is the reverse of rank: rank[v] = r <==> knar[r] = v
    // need MAX_V + 1 because rank = 0 indicates unprocessed
    int knar[MAX_V + 1];

    // tv and tvb functions similar to LOWLINK
    // tv records down the set of vertices we are processing, while
    // tvb is a stack recording the rank of the roots of the subtrees
    // we are processing 
    int tv[MAX_V];
    int tv_sp = 0;
    int tvb[MAX_V];
    int tvb_sp = 0;

    int sv1count = 0;
    // cand: the root element of the current tree
    int cand = 0;
    int in_inner_loop = 0;
    // Loop bound:
    // Each edge is processed once at *extend tv with targets of v*
    // Each vertex is processed at most twice: in the stack once, skip once
    [[buffet::fsm(2 * MAX_V + MAX_E)]]
    while (sv1count < NV) {
        while (rank[cand] != 0) cand++;
        int v = cand;
        nvc = 1;
        rank[v] = 1;
        knar[1] = v;
        cc_push(1);
        tvb_push(0);

        // extend tv with targets of v
        j = input->edgeB[v + 1];
        while (j > input->edgeB[v]) {
            j--;
            tv_push(input->edges[j]);
        }

        while (cc_sp > 0) {
            while (tv_sp > tvb[tvb_sp - 1]) {
                tv_pop(v);
                while (rank[v] > 0 && cc[cc_sp - 1] > rank[v]) {
                    cc_sp--;
                    tvb_sp--;
                }
                if (rank[v] == 0) {
                    in_inner_loop = 1;
                    nvc++;
                    rank[v] = nvc;
                    knar[nvc] = v;
                    cc_push(nvc);
                    tvb_push(tv_sp);
                    
                    // extend tv with targets of v
                    j = input->edgeB[v + 1];
                }
                while (in_inner_loop == 1 && j > input->edgeB[v]) {
                    j--;
                    tv_push(input->edges[j]);
                }
                in_inner_loop = 0;
            }

            output->MSCnum++;
            int tmp = cc[cc_sp - 1];
            while (nvc >= tmp) {
                nvc--;
                rank[knar[nvc + 1]] = NV + output->MSCnum;
                sv1count++;
            }
            cc_sp--;
            tvb_sp--;
        }

    }
    output->MSCnum++;
    for (i = 0; i < MAX_V; i++) {
        if (i < NV) output->MSC[i] = rank[i] - NV;
    }
}
