
#ifndef PREAMBLE_HANDLER_H
#define PREAMBLE_HANDLER_H

#include "file_utils.hpp"

// scan for the usize flag
// Convert all int to uint_32t if usize flag is on
void scan_for_usize_flag(Files& files, State& state) {
  std::cout << "Scanning for usize flag..." << std::endl;
  std::string line;
  while (std::getline(files.src, line)) {
    // if we hit the premarker then exit
    if (line == "PREAMBLE") { break; }
    // otherwise if the line is not empty then emit it to all non-viper files
    else if (line != "") {
      // interpret define as a mapping an update state for viper files
      std::vector<std::string> tokens = tokenize(line);
      if (tokens[0] == "UFLAG" && tokens[1] == "on") {
        state.uflag = true;
      }
    }
  }
}

// scan for the preamble section and emit define lines to all files
void scan_for_preamble(Files& files, State& state, bool pFlag) {
  std::cout << "Scanning for preamble..." << std::endl;
  std::string line;
  while (std::getline(files.src, line)) {
    // if we hit the premarker then exit
    if (line == "PRE-TiTe") { break; }
    // otherwise if the line is not empty then emit it to all non-viper files
    else if (line != "") {
      // write define to c files
      if (!pFlag) { files.tsc << line << std::endl; }
      if (!pFlag) { files.tic << line << std::endl; }
      if (!pFlag) { files.tec << line << std::endl; }
      files.exo << line << std::endl;
      files.inp << line << std::endl;
      // interpret define as a mapping an update state for viper files
      std::vector<std::string> tokens = tokenize(line);
      if (tokens[0] == "#define") {
        state.defines.push_back(tokens[1]);
      }
    }
  }
}

// verbatim copy the section to a specific viper file
void viper_verbatim(std::ifstream& src, std::ofstream& file, std::string terminator) {
  std::string line;
  while (std::getline(src, line)) {
    // if we hit next section then exit
    if (line == terminator) { break; }
    // otherwise if the line is not empty then emit it to all viper files
    else if (line != "") {
      file << line << std::endl;
    }
  }
}

// verbatim copy the section to ALL C files
void c_verbatim(std::ifstream& src, Files& files, std::string terminator) {
  std::string line;
  while (std::getline(src, line)) {
    // if we hit next section then exit
    if (line == terminator) { break; }
    // otherwise if the line is not empty then emit it to all viper files
    else if (line != "") {
      files.tic << line << std::endl;
      files.tec << line << std::endl;
      files.tsc << line << std::endl;
      files.exo << line << std::endl;
    }
  }
}

void compile_preamble(Files& files) {
  std::cout << "Compiling TiTe preamble..." << std::endl;
  viper_verbatim(files.src, files.vie, "PRE-TeTs");
  std::cout << "Compiling TeTs preamble..." << std::endl;
  viper_verbatim(files.src, files.ves, "HELPER");
  std::cout << "Compiling C helper functions..." << std::endl;
  c_verbatim(files.src, files, "IN");
}

// verbatim copy the preamble section to all viper files and end with a "{"
void compile_reqs(Files& files) {
  std::cout << "Compiling viper TiTe requirements..." << std::endl;
  viper_verbatim(files.src, files.vie, "REQ-TeTs");
  std::cout << "Compiling viper TeTs requirements..." << std::endl;
  viper_verbatim(files.src, files.ves, "TI");
  files.vie << "{" << std::endl;
  files.ves << "{" << std::endl;
}

#endif