/* Let arr[i](t) be an access to arr[i] at time t */
/* FIND_SET finds the root and handles path compression */
/* This is done as detailed in the notes (using fixed-points + two permutation networks) */

#define MAX_VERTICES 256
#define MAX_EDGES 32640

// This is a pseudocode for MST_Tb using pequin style.
// This code is NOT pequin compatible! 
#include <stdint.h>

struct In {
	uint32_t num_v;
	uint32_t num_e;
	/* (u, v, w) */
	uint32_t edges[MAX_EDGES][3];
	/* (u, v, w) where first MAX_VERTICES - 1 are in the tree (not sorted by weight) */
	uint32_t sorted_edges[MAX_EDGES][3];
};

struct Out {
	/* (u, v, w) */
	uint32_t tree[MAX_VERTICES - 1][3];
};


void compute(struct In *input, struct Out *output) {
	/* Initialize disjoint set */
	uint32_t parent[MAX_VERTICES];
	uint32_t rank[MAX_VERTICES];

	/* Make Set (For all Vertices) */
	for (uint32_t i = 0; i < MAX_VERTICES; i++) {
		parent[i](0) = i;
		rank[i](0) = 0;
	}

	/* Assume here we have access to a sort routine (permutation network with final order check) */
	assert(is_permutation(edges, sorted_edges));

	i = 0;

	/* Edges in Tree */
	[[buffet::fsm(MAX_VERTICES - 1)]]
	while (i < num_v - 1) {
		/* Free, just aliased here */
		uint32_t u = sorted_edges[i][0];
		uint32_t v = sorted_edges[i][1];
		uint32_t w = sorted_edges[i][2];

		/* Find Set operations handled via new memory construct */
		FIND_SET(u, w);
		FIND_SET(v, w);

		uint32_t u_root = parent[u](w);
		uint32_t v_root = parent[v](w);
		
		assert(u_root != v_root);

		/* Add edge to tree (free), just aliased */
		tree[i] = sorted_edges[i];

		/* disjoint Set Union by rank */
		uint32_t u_rank = rank[u](w);
		uint32_t v_rank = rank[v](w);
		bool swap = (u_rank < v_rank);
		uint32_t min = swap * (u_root - v_root) + v_root;
		uint32_t max = swap * (v_root - u_root) + u_root;
		parent[min](w) = max;
		rank[v](w) += (u_rank == v_rank);

		i++;
	}

	/* Edges not in Tree */
	[[buffet::fsm(MAX_EDGES - MAX_VERTICES + 1)]]
	while (i < num_e) {
		/* Free, just aliased here */
		uint32_t u = sorted_edges[i][0];
		uint32_t v = sorted_edges[i][1];
		uint32_t w = sorted_edges[i][2];

		/* Find Set operations handled via new memory construct */
		FIND_SET(u, w);
		FIND_SET(v, w);
		
		assert(parent[u](w) != parent[v](w));

		i++;
	}
}