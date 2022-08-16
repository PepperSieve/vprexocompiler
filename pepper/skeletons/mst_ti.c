#define MAX_VERTICES 256
#define LOG_VERTICES 8
#define MAX_EDGES 32640

// This is a pseudocode for MST_Ti using pequin style.
// This code is NOT pequin compatible! 
#include <stdint.h>

struct In {
    uint32_t num_v;
    uint32_t num_e;
	/* (u, v, w) */
	uint32_t edges[MAX_EDGES][3];
	/* (u, v, w) sorted by w from least to greatest */
	uint32_t sorted_edges[MAX_EDGES][3];
};

struct Out {
	/* (u, v, w) */
	uint32_t tree[MAX_VERTICES - 1][3];
};


void compute(struct In *input, struct Out *output) {
	/* Bookkeeping */
	uint32_t idx = 0;

	/* Initialize disjoint set	*/
	uint32_t parent[MAX_VERTICES];
	uint32_t rank[MAX_VERTICES];

	/* Make Set (For all Vertices) */
	for (i = 0; i < MAX_VERTICES; i++) {
        if (i < num_v) {
		    parent[i] = i;
		    rank[i] = 0;
        }
	}

	/* Assume here we have access to a sort routine (permutation network with final order check) */
	assert(is_permutation(edges, sorted_edges));

    i = 0;
	/* In the worst case the below while loops take MAX_E * (log_2 MAX_V) iterations */
    [[buffet::fsm(MAX_EDGES * 2 * LOG_VERTICES)]]
	while (i < num_e) {
		/* Free, just aliased here */
		uint32_t u = sorted_edges[i][0];
		uint32_t v = sorted_edges[i][1];

		/* Find Set on u */
			/* 1. Find root of u */
			uint32_t u_tmp = u;
			while(u_tmp != parent[u_tmp]) {
				u_tmp = parent[u_tmp];
			}
			uint32_t u_root = u_tmp;

			/* 2. Path compression from u to root */
			u_tmp = u;
			while(u_tmp != u_root) {
				parent[u_tmp] = u_root;
			}

		/* Find Set on v */
			/* 1. Find root of v */
			uint32_t v_tmp = v;
			while(v_tmp != parent[v_tmp]) {
				v_tmp = parent[v_tmp];
			}
			uint32_t v_root = v_tmp;

			/* 2. Path compression from v to root */
			v_tmp = v;
			while(v_tmp != v_root) {
				parent[v_tmp] = v_root;
			}

		/* If u and v aren't part of the same set (don't share a root) */
		if (u_root != v_root) {
			/* Add edge to tree */
			tree[idx] = sorted_edges[i];
			idx++;

			/* disjoint Set Union by rank */
			bool swap = (rank[u] < rank[v]);
			uint32_t min = swap * (parent[u] - parent[v]) + parent[v];
			uint32_t max = swap * (parent[v] - parent[u]) + parent[u];
			parent[rank[u] < rank[v] ? u : v] = 
			parent[min] = max;
			rank[v] = rank[v] + (rank[u] == rank[v]);
		}
        i++;
	}
}