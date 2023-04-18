#define MAX_N 10
#include <stdint.h>

struct In {
    // l inclusive, r exclusive
    uint32_t l;
    uint32_t r;
    uint32_t x;
    // index of the element s.t. a[ind] = x;
    // if no such element exists, then ind = -1
    uint32_t ind;
    // sup is provided if ind == -1
    // a[sup] < x && a[sup + 1] > x
    // if x < a[l], sup = l - 1
    // if x > a[r - 1], sup = r - 1
    uint32_t sup;
};

struct Out {
    uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
    uint32_t a[MAX_N];
    uint32_t l = input->l;
    uint32_t r = input->r;
    uint32_t x = input->x;
    uint32_t ind = input->ind;
    uint32_t sup = input->sup;
    
    uint32_t al = a[l];
    uint32_t ar = a[r - 1];

    if (x < al || x > ar) assert_zero(ind + 1);
    else if (ind == -1) {
        assert_zero(sup < l);
        assert_zero(sup >= r - 1);
        uint32_t tmp1 = a[sup];
        uint32_t tmp2 = a[sup + 1];
        assert_zero(tmp1 >= x);
        assert_zero(x >= tmp2);                
    } else {
        assert_zero(ind < l);
        assert_zero(ind >= r);
        assert_zero(a[ind] - x);
    }
    output->ind = ind;
}
