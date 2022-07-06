#include <stdint.h>

struct In {
    // both l and r are inclusive
    uint32_t l;
    uint32_t r;
    uint32_t x;
    uint32_t a[MAX_N];
    // index of the element s.t. a[ind] = x;
    // if no such element exists, then ind = -1
    uint32_t ind;
    // sup is provided if ind == -1
    // a[sup] < x && a[sup + 1] > x
    // if x < a[l], sup = l - 1
    // if x > a[r], sup = r
    uint32_t sup;
};

struct Out {
    uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
    uint32_t l = input->l;
    uint32_t r = input->r;
    uint32_t x = input->x;
    uint32_t ind = input->ind;
    uint32_t sup = input->sup;
    
    uint32_t al = input->a[l];
    uint32_t ar = input->a[r];

    assert_zero((ind < l) && (ind != -1));
    assert_zero(ind > r);
    assert_zero(sup < l - 1 || sup > r);
    if (ind != -1) assert_zero(input->a[ind] - x);
    else if (sup == l - 1) assert_zero(x >= al);
    else if (sup == r) assert_zero(x <= ar);
    else {
        assert_zero(sup < l);
        assert_zero(sup >= r);
        uint32_t tmp1 = input->a[sup];
        uint32_t tmp2 = input->a[sup + 1];
        assert_zero(tmp1 >= x);
        assert_zero(x >= tmp2);
    }
    
    output->ind = ind;
}
