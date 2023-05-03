#define MAX_N 160
#define MAX_LOG 8
#include <stdint.h>

struct In {
    // l inclusive, r exclusive
    uint32_t l;
    uint32_t r;
    uint32_t x;
};

struct Out {
    uint32_t ind;
};

void compute(struct In *input, struct Out *output) {
    uint32_t a[MAX_N];
    uint32_t l = input->l;
    uint32_t r = input->r;
    uint32_t x = input->x;
    uint32_t found = 0;
    uint32_t amid, tmp;
    for (tmp = 0; tmp < MAX_LOG; tmp++) {
        if (r > l) {
            uint32_t mid = l + (r - l) << 1;
            amid = a[mid];
            if (amid > x)
                r = mid;
            else
                l = mid + 1;
            if (amid == x) {
                output->ind = mid;
                found = 1;
                l = r;
            }
        }
    }
    if (found == 0) output->ind = -1;
}
