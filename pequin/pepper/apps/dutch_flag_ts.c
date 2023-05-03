#define MAX_N 320
// We assume that the order within the same color does not matter
// Since the standard dutch flag is also does not respect the original order

// NOTE: T_s requires elements of same color to be unique,
// neither T_B nor T_I has the same requirement!

#include <stdint.h>

struct item {
    // color: 0, 1, 2
    int color;
    int content;
};

struct In {
    uint32_t n;
    struct item* a[MAX_N];
    // b provided by the prover
    struct item* b[MAX_N];
    // k1: a[k1] RED && a[k1 + 1] WHITE
    // k2: a[k2] WHITE && a[k2 + 1] BLUE
    // Use -1 and n-1 if some of the colors does not exist
    uint32_t k1, k2;
    // Extra verification to prove permutation
    uint32_t ord_in_A[MAX_N];
    uint32_t ord_in_B[MAX_N];
};

struct Out {
    struct item* b[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int i, ord;
    int n = input->n;
    int k1 = input->k1;
    int k2 = input->k2;
    struct item* tmp;
    assert_zero(k1 < -1);
    assert_zero(k1 > k2);
    assert_zero(k2 >= n);
    for (i = 0; i < MAX_N; i++) {
        if (i < n) {
            //B[i] in A
            ord = input->ord_in_A[i];
            assert_zero(ord < 0 || ord >= n);
            assert_zero(input->b[i] != input->a[ord]);
            //A[i] in B
            ord = input->ord_in_B[i];
            assert_zero(ord < 0 || ord >= n);
            assert_zero(input->a[i] != input->b[ord]);
        }
        tmp = input->b[i];
        if (i <= k1) assert_zero(tmp->color);
        else if (i <= k2) assert_zero(tmp->color - 1);
        else if (i < n) assert_zero(tmp->color - 2);
    }
}
