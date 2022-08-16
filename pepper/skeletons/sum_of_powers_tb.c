#include <stdint.h>

struct In {
    uint32_t r;
    uint32_t k;
    // supplimentary x and y for the answer provided by prover
    // if it does not exist, x[i] must be -1
    uint32_t x[MAX_R + 1];
    uint32_t y[MAX_R + 1];
    // supplimentary x range [x1, x2] and corrresponding y list for each x
    uint32_t x1;
    uint32_t x2;
    uint32_t cor_y[MAX_R + 1];
};

struct Out {
    uint32_t x[MAX_R];
    uint32_t y[MAX_R];
    uint32_t out_size;
};

int pow(int x, int k) {
    int y = 1;
    int i;
    for (i = 0; i < MAX_K; i++) {
        if (k > 0) {
            y *= x;
            k--;
        }
    }
    return y;
}

void compute(struct In *input, struct Out *output){
    int i, j;
    int r = input->r;
    int k = input->k;
    int x1 = input->x1;
    int x2 = input->x2;

    // r >= 0
    // either x1 is 0, or 2 * (x1 - 1)^k < r && x1^k >= r
    assert_zero(x1 < 0);
    if (x1 != 0) assert_zero(2 * pow(x1 - 1, k) >= r);
    assert_zero(2 * pow(x1, k) < r);
    // x2^k <= r && (x2 + 1)^k > r
    assert_zero(pow(x2, k) > r);
    assert_zero(pow(x2 + 1, k) <= r);

    int count = 0;
    for (j = 0; j < MAX_R + 1; j++) {
        // This means that x1 + j is valid
        if (x1 + j <= x2) {
            int tmp1 = pow(x1 + j, k);
            int tmp2 = pow(input->cor_y[j], k);

            // Assert tmp1 + tmp2 <= r
            //        AND (x1 + j)^k + (cor_y[i] + 1)^k > r
            assert_zero(tmp1 + tmp2 > r);
            assert_zero(tmp1 + pow(input->cor_y[j] + 1, k) <= r);

            // if tmp1 + tmp2 == r, we want to verify that
            // x[i] == x1 + j && y[i] == cor_y[j] for some i
            if (tmp1 + tmp2 == r) {
                assert_zero(input->x[count] != x1 + j);
                assert_zero(input->y[count] != input->cor_y[j]);
                count++;
            }    
        }
    }

    output->out_size = count;
}
