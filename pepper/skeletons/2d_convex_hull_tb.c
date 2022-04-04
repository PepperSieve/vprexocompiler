// Given a list of (x, y) coordinates of points in 2D
// Find the Convex Hull
// Assume that no three points lie on the same line
// Assume that the points are already sorted counter-clockwise from the
// bottom-right point

// For every point in the convex hull, provide the coordinate of the next
// convex hull
// If we reach the next point, verify the next of the last point is correct
// If a point is not in convex hull, verify that it together with two adjacent
// points in convex hull forms an inward angle

#include <stdint.h>

#define X_PROD(x1, y1, x2, y2, x3, y3) (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)

struct In {
	uint32_t n;
	uint32_t x[MAX_N];
	uint32_t y[MAX_N];
	// in_c: 0 if point is not in convex hull, 1 otherwise
	uint32_t in_c[MAX_N];
	// use next_xy to record the coordinate of the next point in hull
	// not in use if the point is not on convex hull
	uint32_t next_x[MAX_N];
	uint32_t next_y[MAX_N];
	uint32_t ss;
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
	int ss = input->ss;
	uint32_t stack_x[MAX_N];
	uint32_t stack_y[MAX_N];
	// Record down the first point so we can verify after reaching the last point
	int x0 = input->x[0]; int y0 = input->y[0];
	int next_x = input->next_x[0]; int next_y = input->next_y[0];
	int last_x = x0; int last_y = y0;

	// There must be at least three points in the hull
	assert_zero(ss < 3);

	// The first point must be in the convex hull	
	assert_zero(x0 - input->stack_x[0]);
	assert_zero(y0 - input->stack_y[0]);

	int i, xi, yi, prod;
	// count: which point in stack are we dealing with?
	int count = 0;
	
	for (i = 1; i < MAX_N; i++) {
		if (i < n) {
			xi = input->x[i];
			yi = input->y[i];
			// If in convex hull
			if (input->in_c[i] == 1) {
				// Verify that the point in stack is correct
				count++;
				assert_zero(input->stack_x[count] - xi);
				assert_zero(input->stack_y[count] - yi);

				// Verify that next is correct
				assert_zero(xi - next_x);
				assert_zero(yi - next_y);
				
				// Verify that an outward angle is formed
				next_x = input->next_x[i];
				next_y = input->next_y[i];
				prod = X_PROD(last_x, last_y, xi, yi, next_x, next_y); 
				assert_zero(prod <= 0);

				// Update last
				last_x = xi;
				last_y = yi;
			} 
			else {
				prod = X_PROD(last_x, last_y, xi, yi, next_x, next_y); 
				assert_zero(prod >= 0);
			}
		}
	}

	// Assert that next of last is the first
	assert_zero(x0 - next_x);
	assert_zero(y0 - next_y);

	// make sure that ss is actually correct
	assert_zero(ss - count - 1);

	output->ss = ss;
}
