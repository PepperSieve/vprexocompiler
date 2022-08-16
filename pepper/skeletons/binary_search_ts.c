#include <stdint.h>

struct In {
    // l inclusive, r exclusive
    uint32_t l;
    uint32_t r;
    uint32_t x;
    uint32_t a[MAX_N];
    // index of the element s.t. a[ind] = x;
    // if no such element exists, then ind = -1
    uint32_t ind;
};

struct Out {
    uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
    uint32_t l = input->l;
    uint32_t r = input->r;
    uint32_t x = input->x;
    uint32_t ind = input->ind;
    uint32_t tmp;

    assert_zero(ind < l && ind != -1);
    assert_zero(ind >= r);
    if (ind != -1) assert_zero(input->a[ind] != x);
    else {
        for (tmp = 0; tmp < MAX_N; tmp++) {
            if (tmp >= l) {
                if (tmp < r) {
                    assert_zero(input->a[tmp] == x);
                }
            }
        }
    }
    
    output->ind = ind;
}
