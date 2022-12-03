// This algorithm checks the FIRST MATCH

// For every comparison starting from i, either it is a match, or PAT and
// TXT differs at some index b[i], i.e. txt[i + b] != pat[b]
// Let the prover provide this b[i]. If it is a match, set b[i] to M.
// The claim is that we can let the prover find this b[i] quickly by
// slightly modifying the KMP algorithm. 

#include <stdint.h>

struct In {
    uint32_t M;
    uint32_t N;
    uint32_t pat[MAX_M];
    uint32_t txt[MAX_N];
    // ind and b[i] provided by the prover
    // ind: index of the match
    // If there is no match, set ind to N - M + 1
    // If match, set b[i] to M
    uint32_t ind;
    uint32_t b[MAX_N];
};

struct Out {
    uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
    int ind = input->ind;
    int M = input->M;
    int N = input->N;
    int i, b, t, p;
    assert_zero(ind < 0 || ind > N - M + 1);
    // Verify Mismatch
    for (i = 0; i < MAX_N; i++) {
        if (i < ind) {
            b = input->b[i];
            assert_zero(b < 0 || b >= M);
            t = input->txt[i + b];
            p = input->pat[b];
            assert_zero(t == p);
        }
    }

    // Verify Match
    if (ind != N - M + 1) {
        for (i = 0; i < MAX_M; i++) {
            if (i < M) {
                t = input->txt[ind + i];
                p = input->pat[i];
                assert_zero(t - p);
            }
        }
        output->ind = ind;
    } else output->ind = N;
}
