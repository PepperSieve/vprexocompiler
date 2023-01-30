
#ifndef BIOLERPLATE_H
#define BIOLERPLATE_H

#include "file_utils.hpp"
#include "compiler_state.hpp"

// write boilerplate headers to all files
void write_headers(Files& files) {
  std::cout << "Writing Boilerplate File Headers..." << std::endl;
  // TO DO

  // tic boilerplate
  files.tic << "#include <stdint.h>" << std::endl;
  files.tec << "#include <stdint.h>" << std::endl;
  files.tsc << "#include <stdint.h>" << std::endl;
  files.exo << "#include <stdint.h>" << std::endl;
  files.exo << "#include <stdlib.h>" << std::endl;
  files.exo << "#include <stdio.h>" << std::endl;

  // all c files will have a function to emulate slot notation
  files.tic << "#define slot(A, i) A[i]" << std::endl;
  files.tec << "#define slot(A, i) A[i]" << std::endl;
  files.tsc << "#define slot(A, i) A[i]" << std::endl;
  files.exo << "#define slot(A, i) A[i]" << std::endl;

  // all c files will have a function to emulate mat_slot notation
  files.tic << "#define mat_slot(A, n, i, j) A[i * n + j]" << std::endl;
  files.tec << "#define mat_slot(A, n, i, j) A[i * n + j]" << std::endl;
  files.tsc << "#define mat_slot(A, n, i, j) A[i * n + j]" << std::endl;
  files.exo << "#define mat_slot(A, n, i, j) A[i][j]" << std::endl;
}

// write boilerplate footers to all files
void write_footers(Files& files) {
  std::cout << "Writing Boilerplate File Footers..." << std::endl;
  // TO DO

  // all viper files end with a "}"
  files.vie << "}" << std::endl;
  files.ves << "}" << std::endl;
  // all pequin c files end with a "}"
  files.tic << "}" << std::endl;
  files.tec << "}" << std::endl;
  files.tsc << "}" << std::endl;
  // write exo footer
  files.exo << "}" << std::endl;
  files.exo << "int main() {" << std::endl;
  files.exo << "\tstruct In *input = malloc(sizeof(struct In));" << std::endl;
  files.exo << "\tread_input(input);" << std::endl;
  files.exo << "\tcompute(input);" << std::endl;
  files.exo << "\tfree(input);" << std::endl;
  files.exo << "}" << std::endl;
}

void compile_inp(Files& files, State& state) {
  std::cout << "Compiling Input Generator Code..." << std::endl;
  // TO DO
}

#endif
