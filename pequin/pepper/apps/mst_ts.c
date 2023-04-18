#define MAX_VERTICES 5
#define INV_ACKER_EDGES 3
#define MAX_EDGES 10
#define LOG_VERTICES 3

#include <stdint.h>

struct In {
	int num_v; // number of vertices
	int num_e; // number of edges
	int edges[3 * MAX_EDGES]; // edges are (u, v, weight) (we could speed this up using structs)
	int in_tree[MAX_EDGES]; // tree is boolean (tree[i] == 1 means edge i is included in the MST)
	int edge_witness[MAX_EDGES * MAX_VERTICES]; // each edge with tree[i] == 0 must have a cycle where it is the heaviest (this stores the idx of those edges)
	int edge_dir_witness[MAX_EDGES * MAX_VERTICES]; // if 1 then swap u and v for cycle direction
};

struct Out {
	int tree[3 * (MAX_VERTICES - 1)]; // output MST (edges)
};

void compute(struct In * input, struct Out * output) {
	int idx = 0; // idx in output tree
	int i; // outer loop variable
	int asst = 0; // assert 0 if statements are broken so this is an assert accumulator
	for (i = 0; i < MAX_EDGES; i++) {
		if (i < input->num_e) { // support graphs with fewer edges than max
			// alias edge i
			int u = input->edges[3 * i    ];
			int v = input->edges[3 * i + 1];
			int w = input->edges[3 * i + 2];
			// if in the tree
			if(input->in_tree[i] == 1) {
				// then add edge to output
				output->tree[3 * idx	  ] = u;
				output->tree[3 * idx + 1] = v;
				output->tree[3 * idx + 2] = w;
				idx++;
			} else {
				// otherwise we need to show a cycle where that edge is the heaviest
				int j; // inner loop variable
				int vr = v; // previous vertex
				int d = 0; // boolean "done"
				int oq = i; // previous edge id in cycle
				for(j = 0; j < MAX_VERTICES; j++) {
					if(d < 1) { // cycles can be smaller than MAX_VERTICES so we have this condition to exit early
						// q is the current edge id in the cycle
						int q = input->edge_witness[MAX_VERTICES * i + j];
						// ensure that q is a valid edge id [0, input->num_e)
						if (q < 0) { asst++; }
						if (q >= input->num_e) { asst++; }
						if (q == oq) { asst++; } // ensure no backedges
						if (input->edges[3 * q + 2] >= w) { asst++; } // ensure current edge is lighter than original
						oq = q; // set the previous edge to the current edge
						// we need to check that this edge connects to the previous one
						int eu = input->edges[3 * q]; // alias edge info
						int ev = input->edges[3 * q + 1]; // alias edge info
						if (input->edge_dir_witness[MAX_VERTICES * i + j]) { int tmp = eu; eu = ev; ev = tmp; } // havoc conditionally swap eu and ev
						if (eu != vr) { asst++; } // current u is previous v
						vr = ev; // set next v to current v
						if (u == vr) { d = 1; } // if the current v is the original u then we have completed a cycle
					}
				}
				if (d != 1) { asst++; }
			}
		}
	}
	assert_zero(asst); // assert accumulator
}
