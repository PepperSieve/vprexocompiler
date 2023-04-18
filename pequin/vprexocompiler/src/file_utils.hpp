
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// file utilities
// attempt to open file with name "filename" in write mode
// error out if file cannot be opened
// otherwise return ofstream object
void open_file_w(std::string filename, std::ofstream& file) {
  file.open(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file " << filename << std::endl;
    exit(0);
  }
}

// attempt to open file with name "filename" in read mode
// error out if file cannot be opened
// otherwise return ofstream object
void open_file_r(std::string filename, std::ifstream& file) {
  file.open(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file " << filename << std::endl;
    exit(0);
  }
}

std::vector<std::string> tokenize(std::string& line) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream iss(line);
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

// struct for moving around all files
struct Files {
  // source file
  std::ifstream src;
  // tsc = pequin ts,
  // tic = pequin ti,
  // tec = pequin te,
  // exo = pequin exo (c with gmp)
  // inp = pequin input (h with gmp)
  // vie = viper TiTe proof
  // ves = viper TeTs proof
  std::ofstream tsc, tic, tec, exo, inp, vie, ves;
  // open all required files
  Files (std::string filename) {
    std::cout << "Opening Files..." << std::endl;
    open_file_r(filename + ".vpex", src);
    open_file_w(filename + "_ts.c", tsc);
    open_file_w(filename + "_ti.c", tic);
    open_file_w(filename + "_te.c", tec);
    open_file_w(filename + "_exo.c", exo);
    open_file_w(filename + "_v_inp_gen.h", inp);
    open_file_w(filename + "_TiTe.vpr", vie);
    open_file_w(filename + "_TeTs.vpr", ves);
  }
  // destructor to close all files
  ~Files () {
    src.close();
    tsc.close();
    tic.close();
    tec.close();
    exo.close();
    inp.close();
    vie.close();
    ves.close();
  }
};

#endif
