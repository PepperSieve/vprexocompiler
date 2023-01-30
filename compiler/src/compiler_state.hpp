
#ifndef COMPILER_STATE_H
#define COMPILER_STATE_H

#include <iostream>
#include <string>
#include <unordered_map>

enum Type { INT, ARR, MAT };

struct Var {
  Type type;
  std::string rows, cols;
};

// struct for moving around compiler state
struct State {
  // shared name of all files
  std::string name;
  // input, output, ghost, and local vars and types
  std::unordered_map<std::string, Var> in_vars, out_vars, ghost_vars, local_vars;
  // define rewrite rules
  std::vector<std::string> defines;
  // loop variables for incrementing in viper
  std::vector<std::string> loop_vars;
  // TO DO, control flow extras
  std::string prev_token_zero;
  // boolean to check if we are in loop invariants
  bool in_loop_invariant;
  // prettify output
  int indent;
};

#endif
