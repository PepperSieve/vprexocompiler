#define MAX_VERTICES 5
#define INV_ACKER_EDGES 3
#define MAX_EDGES 10
#define LOG_VERTICES 3

#include <stdint.h>

// commented lines are accounted for in disjoint set data structure
// this just generates a base constraint count and then we can add the theoretical disjoint set test costs

struct In {
  int num_v;
  int num_e;
  /* (u, v, w) */
  //int edges[3 * MAX_EDGES];
  /* (u, v, w) where first MAX_VERTICES - 1 are in the tree (not sorted by weight) */
  int sorted_edges[3 * MAX_EDGES];
  int parent[MAX_VERTICES];
};

struct Out {
  /* (u, v, w) */
  int tree[3 * (MAX_VERTICES - 1)];
};

void compute(struct In * input, struct Out * output) {
  /* Initialize disjoint set */
  int parent[MAX_VERTICES];
  int rank[MAX_VERTICES];

  int i;
  /* Make Set (For all Vertices) */
  for (i = 0; i < MAX_VERTICES; i++) {
    if (i < input->num_v) { 
      //parent[i] = i;
      rank[i] = 0;
    }
  }

  /* Assume here we have access to a sort routine (permutation network with final order check) */
  //assert(is_permutation(edges, sorted_edges));

  /* Edges in Tree */
  for(i = 0; i < MAX_VERTICES; i++) {
    if (i < input->num_v) { 
    /* Free, just aliased here */
    int u = input->sorted_edges[3 * i];
    int v = input->sorted_edges[3 * i + 1];
    //int w = input->sorted_edges[3 * i + 2];
    //int u = i; int v = MAX_VERTICES - i - 1; int w = i + 2;
    //int u, v, w;
    //int u = input->sorted_edges[3 * i];
    /* Find Set operations handled via new memory construct */
    //FIND_SET(u, w);
    //FIND_SET(v, w);

    int u_root = parent[u];
    int v_root = parent[v];

    assert_zero(u_root == v_root);

    /* Add edge to tree (free), just aliased */
    //output->tree[3 * i    ] = input->sorted_edges[3 * i];
    //output->tree[3 * i + 1] = input->sorted_edges[3 * i + 1];
    //output->tree[3 * i + 2] = input->sorted_edges[3 * i + 2];

    //output->tree[3 * i    ] = u;
    //output->tree[3 * i + 1] = v;
    //output->tree[3 * i + 2] = w;
    
    /* disjoint Set Union by rank */
    int u_rank = rank[u];
    int v_rank = rank[v];
    //if(u_rank < v_rank) {
      //parent[u_root] = v_root;
    //} else {
      //parent[v_root] = u_root;
    //}
    if(u_rank == v_rank) {
      rank[v]++;
    }
    }
  }

  /* Edges not in Tree */
  for(i = MAX_VERTICES; i < MAX_EDGES; i++) {
    if(i < input->num_e) {
    /* Free, just aliased here */
    int u = input->sorted_edges[3 * i];
    int v = input->sorted_edges[3 * i + 1];
    //int w = input->sorted_edges[3 * i + 2];
    //int u = i; int v = 1;
    /* Find Set operations handled via new memory construct */
    //FIND_SET(u, w);
    //FIND_SET(v, w);

    assert_zero(parent[u] == parent[v]);
    }  
  }
}