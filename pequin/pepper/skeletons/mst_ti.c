
#include <stdint.h>

// this is a base constraint count, we need to add the cost of a permutation check on edges to our final accounting

struct In {
  int num_v;
  int num_e;
  /* (u, v, w) */
  //int edges[3 * MAX_EDGES];
  /* (u, v, w) sorted by w from least to greatest */
  int sorted_edges[3 * MAX_EDGES];
};

struct Out {
  /* (u, v, w) */
  int tree[3 * (MAX_VERTICES - 1)];
};

void compute(struct In * input, struct Out * output) {
  /* Bookkeeping */
  int idx = 0;

  /* Initialize disjoint set	*/
  int parent[MAX_VERTICES];
  int rank[MAX_VERTICES];

  /* Make Set (For all Vertices) */
  int i;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (i < input->num_v) {
      parent[i] = i;
      rank[i] = 0;
    }
  }

  /* Assume here we have access to a sort routine (permutation network with final order check) */
  //assert(is_permutation(edges, sorted_edges));

  i = 0;
  /* In the worst case the below, the inner while loops take MAX_E * INV_ACKER_EDGES iterations */
  [[buffet::fsm(9 * MAX_EDGES * INV_ACKER_EDGES)]]
  for (i = 0; i < MAX_EDGES; i++) {
    if (i < input->num_e) {
    int j;
    /* Free, just aliased here */
    int u = input->sorted_edges[3 * i];
    int v = input->sorted_edges[3 * i + 1];

    /* Find Set on u */
    /* 1. Find root of u */
    int u_tmp = u;
    for (i = 0; i < LOG_VERTICES; i++) {
      if(u_tmp == parent[u_tmp]) { break; }
      u_tmp = parent[u_tmp];
    }
    //assert_zero(u_tmp != parent[u_tmp]);
    int u_root = u_tmp;

    /* 2. Path compression from u to root */
    u_tmp = u;
    for (i = 0; i < LOG_VERTICES; i++) {
      if (u_tmp == u_root) { break; }
      parent[u_tmp] = u_root;
    }

    /* Find Set on v */
    /* 1. Find root of v */
    int v_tmp = v;
    for (i = 0; i < LOG_VERTICES; i++) {
      if(v_tmp == parent[v_tmp]) { break; }
      v_tmp = parent[v_tmp];
    }
    //assert_zero(v_tmp != parent[v_tmp]);
    int v_root = v_tmp;

    /* 2. Path compression from v to root */
    v_tmp = v;
    for (i = 0; i < LOG_VERTICES; i++) {
      if (v_tmp == v_root) { break; }
      parent[v_tmp] = v_root;
    }

    /* If u and v aren't part of the same set (don't share a root) */
    if (u_root != v_root) {
      /* Add edge to tree */
      output->tree[3 * idx]     = input->sorted_edges[3 * i];
      output->tree[3 * idx + 1] = input->sorted_edges[3 * i + 1];
      output->tree[3 * idx + 2] = input->sorted_edges[3 * i + 2];
      idx++;

      /* disjoint Set Union by rank */
      int u_rank = rank[u];
      int v_rank = rank[v];
      if(u_rank < v_rank) {
        parent[u_root] = v_root; 
      } else {
        parent[v_root] = u_root;
      }
      if(u_rank == v_rank) {
        rank[v]++;
      }
    }
    i++;
  }}
}