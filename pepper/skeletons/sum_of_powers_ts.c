#include <stdint.h>

struct In {
    uint32_t r;
    uint32_t k;
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
    int x, y;
    int r = input->r;
    int k = input->k;
    int count = 0;

    for (x = 0; x < MAX_R + 1; x++) {
        if (x <= r) {
            for (y = 0; y < MAX_R + 1; y++) {
                if (y <= x) {
                    int tmp1 = pow(x, k);
                    int tmp2 = pow(y, k);

                    if (tmp1 + tmp2 == r) {
                        output->x[count] = x;
                        output->y[count] = y;
                        count++;
                    }    
                }
            }
        }
    }

    output->out_size = count;
}
