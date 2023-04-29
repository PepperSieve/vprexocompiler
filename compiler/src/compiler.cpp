
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
  // aFlag: use counter to get around pequin assert_zero bug
  bool aFlag = file_name == "-a" || file_name == "-ap" || file_name == "-pa";
  // pFlag: ignore everything in preamble
  bool pFlag = file_name == "-p" || file_name == "-ap" || file_name == "-pa";
  if (argc == 3 && (file_name == "-a" || file_name == "-p" || file_name == "-ap" || file_name == "-pa")) {
    file_name = std::string(argv[2]);
  }
  // create a Files struct to hold all the files
  Files files(file_name);
  // create a State struct to hold current state of the compiler for persistance between sections
  State state;
  state.name = file_name.substr(file_name.rfind('/') + 1);
  state.uflag = false;
  // read src_file line by line (section by section)
  scan_for_usize_flag(files, state);
  scan_for_preamble(files, state, pFlag);
  // write boilerplate headers of the files
  write_headers(files);
  // compile all the sections
  compile_preamble(files);
  compile_input(files, state, file_name);
  compile_output(files, state);
  compile_reqs(files);
  compile_Ti(files, state);
  write_ghost_loads(files.tec, state);
  write_ghost_loads(files.tsc, state);
  write_ghost_havocs(files.ves, state);
  compile_Te(files, state, aFlag);
  write_exo_emissions(files.exo, state);
  compile_Ts(files, state, aFlag);
  compile_Inp(files);
  // TO DO, compile input generation code
  compile_inp(files, state);
  // map local variables to output variables in pequin c files
  write_output(files, state);
  // write boilerplate footers of the files
  write_footers(files);
  // close all files and exit gracefully
  return 0;
}
