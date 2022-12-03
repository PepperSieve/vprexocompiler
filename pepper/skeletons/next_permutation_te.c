#include <stdint.h>

struct In {
    uint32_t n;
    uint32_t c[MAX_N];
    uint32_t cc[MAX_N];
    // D list, i and j provided by the prover
    // Value of c[i], c[j] provided by the prover..
    uint32_t d[MAX_N];
    uint32_t i;
    uint32_t j;
    uint32_t ci, cip1, cj, cjp1;
};

struct Out {
    uint32_t d[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int i = input->i;
    int j = input->j;
    int n = input->n;
    int k, cor;
    int ci = input->ci;
    int cj = input->cj;
    int cip1 = input->cip1;
    int cjp1 = input->cjp1;

    // Copy C to CC and modify CC
    for (k = 0; k < MAX_N; k++) {
        if (k < n && k != i && k != j) assert_zero(input->cc[k] != input->c[k]);
    }

    // Verify that i and j are correct
    // 0 <= i < j < n
    assert_zero(0 > i);
    assert_zero(i >= j);
    assert_zero(j >= n);

    // c[i] < c[i+1] && c[j] > c[i] && c[j+1] < c[i]
    assert_zero(ci >= cip1);
    assert_zero(cj <= ci);
    if (j != n - 1) assert_zero(cjp1 >= ci);

    // Swap c[i] and c[j]
    for (k = 1; k < MAX_N; k++) {
        if (k == i) {
            assert_zero(input->c[k] - ci);
            assert_zero(input->cc[k] - cj);
        }
        if (k == i + 1) assert_zero(input->c[k] - cip1);
        if (k == j) {
            assert_zero(input->c[k] - cj);
            assert_zero(input->cc[k] - ci);
        }
        if (k == j + 1 && k != n) assert_zero(input->c[k] - cjp1);
    }

    // if k < i, d[k] = cc[k]
    // otherwise, d[k] = cc[n - k + 1]
    for (k = 0; k < MAX_N; k++) {
        if (k < n) {
            if (k <= i) cor = k;
            else cor = n - k + i;
            assert_zero(input->d[k] != input->cc[cor]);

            // forall k > i, c[k] > c[k+1]
            if (k != 0 && k - 1 > i)
                assert_zero(input->c[k-1] <= input->c[k]);
        }
    }    
}
