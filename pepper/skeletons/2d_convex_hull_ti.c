// Given a list of (x, y) coordinates of points in 2D
// Find the Convex Hull
// Assume that no three points lie on the same line
// Assume that the points are already sorted counter-clockwise from the
// bottom-right point

#include <stdint.h>

#define X_PROD(x1, y1, x2, y2, x3, y3) (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)

struct In {
    uint32_t n;
    uint32_t x[MAX_N];
    uint32_t y[MAX_N];
};

struct Out {
    // Stack Pointer
    uint32_t ss;
    uint32_t stack_x[MAX_N];
    uint32_t stack_y[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int n = input->n;
    // Use x1, y1, x2, y2 to minimize RAM access
    int i, next_x, next_y, x1, y1, x2, y2, prod;
    x1 = input->x[0]; output->stack_x[0] = x1;
    y1 = input->y[0]; output->stack_y[0] = y1;
    x2 = input->x[1]; output->stack_x[1] = x2;
    y2 = input->y[1]; output->stack_y[1] = y2;
    int ss = 2;
    // Have to use buffet due to nested loops
    [[buffet::fsm(2 * MAX_N)]]
    for (i = 2; i < n; i++) {
        next_x = input->x[i];
        next_y = input->y[i];

        prod = X_PROD(x1, y1, x2, y2, next_x, next_y);
        while (prod < 0) {
            ss--;
            x2 = x1;
            y2 = y1;
            // We actually don't need this if condition,
            // but pequin won't let me pass without adding it.
            if (ss >= 2) {
                x1 = output->stack_x[ss - 1];
                y1 = output->stack_y[ss - 1];
            }
            prod = X_PROD(x1, y1, x2, y2, next_x, next_y);
        }
        output->stack_x[ss] = next_x;
        output->stack_y[ss] = next_y;
        ss++;
        x1 = x2;
        y1 = y2;
        x2 = next_x;
        y2 = next_y;
    }
    output->ss = ss;
}
