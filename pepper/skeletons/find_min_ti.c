// Find and mark all the smallest elements in A
#include <stdint.h>

struct In {
    uint32_t n;
    uint32_t a[MAX_N];
};

struct Out {
    uint32_t min;
    uint32_t b[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int i;
    int min = input->a[0];
    int n = input->n;
    for (i = 1; i < MAX_N; i++) {
        if (i < n) {
            if (input->a[i] < min) {
                min = input->a[i];
            }
        }
    }
    output->min = min;
    for (i = 0; i < MAX_N; i++) {
        if (i < n) {
            if (input->a[i] == min) output->b[i] = 1;
            else output->b[i] = 0;
        }
    }
}
