// Given a list of (x, y) coordinates of points in 2D
// Find the Convex Hull
// Assume that no three points lie on the same line
// Assume that the points are already sorted counter-clockwise from the
// bottom-right point

// For every point, find its previous and next point in hull
// If a point is not in convex hull, verify that it together with two adjacent
// points in convex hull forms an inward angle,
// otherwise it should form an outward angle.

#include <stdint.h>

#define X_PROD(x1, y1, x2, y2, x3, y3) (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)

struct In {
    uint32_t n;
    uint32_t x[MAX_N];
    uint32_t y[MAX_N];
    // in_c: 0 if point is not in convex hull, 1 otherwise
    uint32_t in_c[MAX_N];
    // stack_x and stack_y provided by the prover
    uint32_t stack_x[MAX_N];
    uint32_t stack_y[MAX_N];
};

struct Out {
    // Stack Pointer
    uint32_t ss;
    uint32_t stack_x[MAX_N];
    uint32_t stack_y[MAX_N];
};

void compute(struct In *input, struct Out *output) {
    int n = input->n;
    uint32_t stack_x[MAX_N];
    uint32_t stack_y[MAX_N];
    // Record down the first point so we can verify after reaching the last point
    int x0 = input->x[0]; int y0 = input->y[0];

    // The first point must be in the convex hull    
    assert_zero(x0 - input->stack_x[0]);
    assert_zero(y0 - input->stack_y[0]);

    int i, j, xi, yi, prod;
    int last, next, last_x, last_y, next_x, next_y;
    // count: which point in stack are we dealing with?
    int count = 0;
    
    for (i = 1; i < MAX_N; i++) {
        if (i < n) {
            xi = input->x[i];
            yi = input->y[i];
            last = i - 1;
            next = i + 1;
            // We shouldn't need this if statement but pequin won't
            // let me pass without it, as in_c might be out of bounds 
            if (next == MAX_N) next = 0;
            // find last and next
            for (j = 0; j < MAX_N; j++) {
                if (input->in_c[last] != 1) last--;
                if (next < n && input->in_c[next] != 1) next++;
            }
            if (next == n) next = 0;
            // Verify angle: > 0 if in hull, < 0 if not in hull
            last_x = input->x[last];
            last_y = input->y[last];
            next_x = input->x[next];
            next_y = input->y[next];
            prod = X_PROD(last_x, last_y, xi, yi, next_x, next_y); 
            if (input->in_c[i] == 1) {
                count++;
                assert_zero(input->stack_x[count] - xi);
                assert_zero(input->stack_y[count] - yi);
                
                assert_zero(prod <= 0);
            } else
                assert_zero(prod >= 0);
        }
    }

    output->ss = count + 1;
}
