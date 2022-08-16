#include <stdint.h>

// N --> Number of elements in the sequence
// M --> Number of recurrence relations

#define f(i, x) (2 * i + 3) * x
#define fLOAD(i, x) input->f_x[i * MAX_N + x]

struct In {
    uint32_t seq_0;
    uint32_t n;
    // Actual result seq provided by the prover
    uint32_t seq[MAX_N];
    // seq_last records the value of seq[n - 1], provided by the prover
    uint32_t seq_last;
    // Prev_k[i] stores the index of the value that generates i
    uint32_t prev_k[MAX_N];
    // Prev_k[i] stores the equation used to generates i
    uint32_t prev_j[MAX_N];
};

struct Out {
    uint32_t seq[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int seq_0 = input->seq_0;
    int n = input->n;
    int seq_last = input->seq_last;
    int k, j, k1, j1, k2;
    int seqk = input->seq[0];
    int seqk1;
    int fjk2;
    assert_zero(seqk - seq_0);

    for (k = 1; k < MAX_N; k++) {
        if (k < n) {
            seqk = input->seq[k];
            k1 = input->prev_k[k];
            j1 = input->prev_j[k];
            seqk1 = input->seq[k1];
            // Verify correctness of prev
            assert_zero(k1 < 0); assert_zero(k1 >= k);
            assert_zero(j1 < 0); assert_zero(j1 >= M);
            assert_zero(f(j1, seqk1) - seqk);

            for (j = 0; j < M; j++) {
                for (k2 = 0; k2 < MAX_N; k2++) {
                    if (k2 < k) {
                        fjk2 = f(j, input->seq[k2]);
                        assert_zero(fjk2 > input->seq[k - 1] && fjk2 < seqk);
                    }
                }
            }
        }
    }
}
