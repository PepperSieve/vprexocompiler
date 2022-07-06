#include <stdint.h>

#define Arr(i, k) input->A[i * MAX_N + k]
#define Ind(i, k) input->ind_in_b[i * MAX_N + k]

struct In {
    uint32_t L;
    uint32_t N[MAX_L];
    uint32_t A[MAX_L * MAX_N];
    // Merged list B and its length provided by the prover
    uint32_t B[MAX_L * MAX_N];
    uint32_t b_len;
    // forall i, B[i] == A[k[i], j[i]]
    uint32_t k[MAX_L * MAX_N];
    uint32_t j[MAX_L * MAX_N];
    // forall p, q, A[p, q] == B[ind_in_b[p, q]]
    uint32_t ind_in_b[MAX_L * MAX_N];
};

struct Out {
    uint32_t C[MAX_L * MAX_N];
};

void compute(struct In *input, struct Out *output) {
    uint32_t L = input->L;
    uint32_t b_len = input->b_len;
    int i, nk, akj, ki, ji, nki;
    // B is increasing and forall i, B[i] in A
    for (i = 0; i < MAX_L * MAX_N; i++) {
        if (i < b_len) {
            ki = input->k[i];
            ji = input->j[i];
            nki = input->N[ki];
            // Assert B is increasing
            assert_zero(i != 0 && input->B[i - 1] >= input->B[i]);
            // 0 <= k_i < L
            assert_zero(0 > ki); assert_zero(ki >= L);
            // 0 <= j_i < A[k_i].len
            assert_zero(0 > ji); assert_zero(ji >= nki);
            // B[i] == A[k_i, j_i]
            assert_zero(input->B[i] != Arr(ki, ji));
        }
    }
    int i1, i2, ind12;
    // forall i1, i2, A[i1, i2] in B
    for (i1 = 0; i1 < MAX_L; i1++) {
        if (i1 < L) {
            for (i2 = 0; i2 < MAX_N; i2++) {
                if (i2 < input->N[i1]) {
                    ind12 = Ind(i1, i2);
                    assert_zero(0 > ind12); assert_zero(ind12 >= b_len);
                    assert_zero(Arr(i1, i2) != input->B[ind12]);
                }
            }
        }
    }
}
