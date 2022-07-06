#include <stdint.h>

struct item {
    // color: 0, 1, 2
    int color;
    int content;
};

struct In {
    uint32_t n;
    struct item* a[MAX_N];
};

struct Out {
    struct item* b[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int i;
    int j = 0;
    int k = input->n;
    struct item* tmp;
    int color, it;
    for (i = 0; i < MAX_N; i++) {
        if (i < k) output->b[i] = input->a[i];
    }
    i = 0;
    struct item* bj;
    for (it = 0; it < MAX_N; it++) {
        if (j <= k) {
            bj = output->b[j];
            color = bj->color;
            if (color == 0) {
                tmp = output->b[i];
                output->b[i] = bj;
                output->b[j] = tmp;
                i++;
                j++;
            } else if (color == 2) {
                tmp = bj;
                output->b[j] = output->b[k];
                output->b[k] = tmp;
                k--;
            } else j++;
        }
    }
}
