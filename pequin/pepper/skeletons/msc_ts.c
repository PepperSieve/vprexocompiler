#include <stdint.h>

#define stack_push(level, head, next, remn) level_s[sp] = level; head_s[sp] = head; next_s[sp] = next; remn_s[sp] = remn; sp++;
#define stack_pop(level, head, next, remn) sp--; level = level_s[sp]; head = head_s[sp]; next = next_s[sp]; remn = remn_s[sp];

struct In {
    int NV;
    // Number of edges provided by the USER
    int NE;
    // Record down the number of edges in the extra slot of edgeB
    int edgeB[MAX_V + 1];
    // Starting vertex of each edge, provided by the USER
    int edgeV[MAX_E];
    int edges[MAX_E];

    // New_Cycl[i] = 0 ==> T[i] is the beginning of a circular path to verify a new MSC
    int cycl[MAX_EXP]; 
    // Array record circular paths to prove each MSC
    int T[MAX_EXP];
    // Edge array to supplement the proof for T
    int E[MAX_EXP];
};

struct Out {
    // We expect the value of MSC already provided by the prover
    int MSCnum;
    int MSC[MAX_V];
};

void compute(struct In *input, struct Out *output) {
    int NV = input->NV;
    int NE = input->NE;
    int MSCnum = output->MSCnum;

    int i, j, v;

    // ----
    // EnsureDisjoint()
    // Part of it is incorporated into EnsureStrong()
    // occ is used to verify that every vertex indeed appeared
    int occ[MAX_V];
    for (i = 0; i < MAX_V; i++) occ[i] = 0;
    // ----

    // ----
    // EnsureStrong()
    
    // which MSC are we currently in? initialize to 0
    int cur_msc = 0;
    // Verify the first node in T is in MSC 0
    int cur_head = input->T[0];
    assert_zero(output->MSC[0]);
    // First edge starts from node 0
    assert_zero(input->edgeV[0]);

    for (i = 1; i < MAX_EXP; i++) {
        if (cur_msc < MSCnum) {
            v = input->T[i];
            occ[v] = 1;
            // A new MSC is formed
            if (input->cycl[i] == 0) {
                // Make a new MSC
                cur_msc++;
                // Last edge ends at head
                assert_zero(input->edgeB[i-1] - cur_head);
                // Reset head
                cur_head = v;
            } else {
                // Last edge ends at current node
                assert_zero(input->edgeB[i-1] - v);
            }
            // Current edge starts from current node
            if (cur_msc != MSCnum) {
                assert_zero(input->edgeV[i] - v);
            }
        }
    }
    // ----

    // Verify that every vertex appears
    for (i = 0; i < MAX_V; i++) {
        if (i < NV) assert_zero(occ[i] == 1);
    }

    // ----
    // EnsureOutGoingToOlder
    cur_msc = output->MSC[0];
    i = 0; j = 0;
    int tmp;
    int ebi0 = input->edgeB[0];
    int ebi1 = input->edgeB[1];
    for (tmp = 0; tmp < MAX_E + MAX_V; tmp++) {
        if (i < NV) {
            if (j == ebi1) {
                i++;
                cur_msc = output->MSC[i];
                ebi0 = ebi1;
                if (i < NV) ebi1 = input->edgeB[i + 1];
            } else {
                assert_zero(output->MSC[input->edges[j]] > cur_msc);
                j++;
            }
        }
    }
    // ----
    output->MSCnum = MSCnum;

}
