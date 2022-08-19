#include <stdint.h>

#define stack_push(level, head, next, remn) level_s[sp] = level; head_s[sp] = head; next_s[sp] = next; remn_s[sp] = remn; sp++;
#define stack_pop(level, head, next, remn) sp--; level = level_s[sp]; head = head_s[sp]; next = next_s[sp]; remn = remn_s[sp];

struct In {
    uint32_t NV;
    // Number of edges provided by the USER
    uint32_t NE;
    // Record down the number of edges in the extra slot of edgeB
    uint32_t edgeB[MAX_V + 1];
    // Starting vertex of each edge, provided by the USER
    uint32_t edgeV[MAX_E];
    uint32_t edges[MAX_E];

    // Array representing the tree structure, bounded by V * log(V)
    // Fill unused slots with values >= NV. The prover can and may do so in the middle of the array
    // and it won't affect the outcome as long as the valid slots are correct.
    int T[2 * MAX_V - 1];
    // The next three arrays are only used when we encounter a pcomp.
    // They record respectively # of remaining slots,
    // index of the receiving vertex, and index of the outgoing vertex
    uint32_t P_remn[2 * MAX_V - 1];
    uint32_t P_recv[2 * MAX_V - 1];
    uint32_t P_outg[2 * MAX_V - 1];
    // Like msc_sp_level, a list of what the next vertex in the level would be
    // next_T = NV if the current vertex is the last vertex in the level
    uint32_t next_T[2 * MAX_V - 1];
    // Edge array to supplement the proof for T
    uint32_t E[2 * MAX_V - 1];
};

struct Out {
    // We expect the value of MSC already provided by the prover
    uint32_t MSCnum;
    uint32_t MSC[MAX_V];
};

void compute(struct In *input, struct Out *output) {
    int NV = input->NV;
    int NE = input->NE;

    // We need a stack to keep track of the level, head vertex, next vertex,
    // and # of remaining slots in the pcomp
    int level_s[MAX_V];
    int head_s[MAX_V];
    int next_s[MAX_V];
    int remn_s[MAX_V];
    int sp = 0;
    // However, we might not need a stack for every level. If we are processing
    // the last child of the tree, we do not need to store the information of
    // the parent anymore. So we differentiate this by using a variable level
    // to record the actual depth, and the stack to store only useful information.
    int level = 0;
    int i, j;

    // ----
    // EnsureDisjoint()
    // Part of it is incorporated into EnsureStrong()
    // occ is used to verify that every vertex indeed appeared
    int occ[MAX_V];
    for (i = 0; i < MAX_V; i++) occ[i] = 0;
    // ----

    // ----
    // EnsureStrong()
    
    // local variables head, next, and remn so we don't have to modify the stack that often
    int head = -1;
    int next = -1;
    int remn = -1;
    // which MSC are we currently in? initialize to -1
    int cur_msc = -1;

    int recv, outg, v, e;
    for (i = 0; i < 2 * MAX_V - 1; i++) {
        if (input->T[i] < NV) {

            // if we are at level 0, we reach a new MSC (tree)
            if (level == 0) cur_msc++;
            
            int v_recv, v_outg, remn_last;

            // Case 1: we meet a new pcomp, represented by a negative number
            if (input->T[i] < 0) {
                remn_last = remn - input->P_remn[i] - 1;
                remn = input->P_remn[i];
                recv = input->P_recv[i];
                outg = input->P_outg[i];
                assert_zero(recv <= i); assert_zero(outg <= i);
                assert_zero(recv > i + remn); assert_zero(outg > i + remn);
                // No need to bound check v_recv and v_outg because otherwise it cannot satisfy an edge
                v_recv = input->T[recv];
                v_outg = input->T[outg];
            }
            
            // Case 2: we meet a singleton vertex (ncomp)
            else {
                v = input->T[i];
                // if we are at level 0, we reach a new MSC (singleton vertex)
                // Verify that MSC[v] == current MSC, also record that v appears
                assert_zero(output->MSC[v] - cur_msc);
                occ[v] = 1;
                v_recv = v;
                v_outg = v;
                remn--;
            }

            // if we are at level 0, we are done. Otherwise record information to prove cyclic
            // This part is shared by both cases, albeit with different v_recv and v_outg
            if (level != 0) {
                if (head == -1) head = v_recv;
                if (next != -1) assert_zero(v_recv - next);
                // Prove the supplementary E edge is correct
                if (input->next_T[i] == NV) next = head;
                else next = input->next_T[i];
                e = input->E[i];
                assert_zero(e < 0); assert_zero(e >= NE);
                assert_zero(input->edgeV[e] - v_outg);
                assert_zero(input->edges[e] - next);
            }

            // Prepare for entering next level if we are in a pcomp
            if (input->T[i] < 0) {
                // store current level in stack if needed
                // one should notice that if we are at level 0, then remn_last will always be < 0 at this state
                if (remn_last != 0) {   
                    stack_push(level, head, next, remn_last);
                }
                level++;
                head = -1;
                next = -1;
            }
         
            // Reset level if remn reaches 0
            if (level != 0 && remn == 0) {
                // Verify the "next" of the last vertex is the "head"
                assert_zero(next - head);
                stack_pop(level, head, next, remn);
            }
        }
    }

    assert_zero(level);
    // Verify that every vertex appears
    for (i = 0; i < MAX_V; i++) {
        if (i < NV) assert_zero(occ[i] == 1);
    }

    // ----
    // EnsureOutGoingToOlder
    output->MSCnum = cur_msc + 1;
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
                }
            else {
                assert_zero(output->MSC[input->edges[j]] > cur_msc);
                j++;
            }
        }
    }
    // ----
}
