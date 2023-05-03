#define MAX_N 10
#define MAX_FAC 3628800
#include <stdint.h>

struct In {
    uint32_t n;
    uint32_t c[MAX_N];
    uint32_t d[MAX_N];
};

struct Out {
    uint32_t d[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int n = input->n;
    int i; int ci;
    int j;
    // Prove that d is a permutation of c
    int is_permutation = 0;

    // Use cc to list all permutations of c
    int st[MAX_N];
    int cc[MAX_N];
    int tmp;
    for (i = 0; i < MAX_N; i++) {
        st[i] = 0;
        cc[i] = input->c[i];
    }
    // i acts similarly to a stack pointer
    i = 1;
    int k;
    for (k = 0; k < MAX_FAC; k++) {
        if (i < n) {
            if (st[i] < i) {
                if (i % 2 == 0) {
                    // swap(cc[0],cc[i])
                    tmp = cc[0];
                    cc[0] = cc[i];
                    cc[i] = tmp;
                } else {
                    // swap(cc[st[i]], cc[i])
                    tmp = cc[st[i]];
                    cc[st[i]] = cc[i];
                    cc[i] = tmp;
                }
                
                // Compare with c
                int lte_c = -1;
                j = 0;
                for (tmp = 0; tmp < MAX_N; tmp++) {
                    if (lte_c == -1 && tmp < n && cc[tmp] == input->c[tmp]) j++;
                    else lte_c = 0;
                }
                if (j == n || cc[j] < input->c[j]) lte_c = 1;
                // Compare with d
                int gte_d = -1;
                j = 0;
                for (tmp = 0; tmp < MAX_N; tmp++) {
                    if (gte_d == -1 && tmp < n && cc[tmp] == input->d[tmp]) j++;
                    else gte_d = 0;
                }
                if (j == n) is_permutation = 1;
                if (j == n || cc[j] > input->d[j]) gte_d = 1;

                // Either cc <= c, or cc >= d.
                assert_zero(lte_c + gte_d - 1);

                // Swap has occurred ending the for-loop. Simulate the increment of the for-loop counter
                st[i]++;
                // Simulate recursive call reaching the base case by bringing the pointer to the base case analog in the array
                i = 1;
            } else {
                // Calling generate(i+1, A) has ended as the for-loop terminated. Reset the state and simulate popping the stack by incrementing the pointer.
                st[i] = 0;
                i++;
            }
        }
    }

    assert_zero(is_permutation - 1);
}