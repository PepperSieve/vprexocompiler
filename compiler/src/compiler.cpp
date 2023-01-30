
// g++ -std=c++11 src/compiler.cpp -o bin/compiler
// ./bin/compiler ./data/binary_search

#include "file_utils.hpp"
#include "compiler_state.hpp"
#include "boilerplate.hpp"
#include "preamble_handler.hpp"
#include "inout_handler.hpp"
#include "ties_handler.hpp"

// main function takes in a file name without extension as a command line argument
// acts as the driver for the compiler
// we assume the file is in the correct format with clearly defined sections
int main(int argc, char* argv[]) {
  // check if the user has provided a file name
  if (argc < 2) {
    std::cerr << "Please provide a file name (without extension)" << std::endl;
    std::cerr << "Use the optional flag -a to collect all pequin assert_zero statements into one" << std::endl;
    return 0;
  }
  std::string file_name = std::string(argv[1]);
  bool aFlag = (std::string(argv[1]) == "-a") || (argc == 3 && std::string(argv[2]) == "-a");
  if (argc == 3 && std::string(argv[1]) == "-a") { file_name = std::string(argv[2]); }
  // create a Files struct to hold all the files
  Files files(file_name);
  // create a State struct to hold current state of the compiler for persistance between sections
  State state;
  state.name = std::string(argv[1]).substr(std::string(argv[1]).rfind('/') + 1);
  // read src_file line by line (section by section)
  scan_for_preamble(files, state);
  // write boilerplate headers of the files
  write_headers(files);
  // compile all the sections
  compile_preamble(files);
  compile_input(files, state);
  compile_output(files, state);
  compile_reqs(files);
  compile_Ti(files, state);
  write_ghost_loads(files.tec, state);
  write_ghost_loads(files.tsc, state);
  write_ghost_havocs(files.ves, state);
  compile_Te(files, state, aFlag);
  write_exo_emissions(files.exo, state);
  compile_Ts(files, state, aFlag);
  // TO DO, compile input generation code
  compile_inp(files, state);
  // map local variables to output variables in pequin c files
  write_output(files, state);
  // write boilerplate footers of the files
  write_footers(files);
  // close all files and exit gracefully
  return 0;
}
