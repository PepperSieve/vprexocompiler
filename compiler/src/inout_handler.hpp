
#ifndef INOUT_HANDLER_H
#define INOUT_HANDLER_H

#include "file_utils.hpp"
#include "compiler_state.hpp"
#include <sstream>
#include <string>

// as written this gets tripped up by lines with just spaces, or malformed lines, refactor later
void load_vars(Files& files, std::unordered_map<std::string, Var>& var_map, std::string terminator) {
  // read the section line by line until we hit the next section (terminator)
  std::string line;
  while (std::getline(files.src, line)) {
    // if we hit the next section then exit
    if (line == terminator) { break; }
    // otherwise if the line is not empty then parse it
    else if (line != "") {
      // split line into tokens by spaces
      std::istringstream iss(line);
      std::string stype, sname, srows, scols;
      Type type;
      iss >> stype >> sname;
      if (stype == "mat") {
        iss >> srows >> scols;
        type = MAT;
      } else if (stype == "arr") {
        iss >> srows;
        scols = "";
        type = ARR;
      } else if (stype == "int") {
        srows = "";
        scols = "";
        type = INT;
      } else if (stype == "struct") {
        // Ugly workaround that stores the entire line as variable name
        sname = line;
        srows = "";
        scols = "";
        type = CUSTOM;        
      } else {
        std::cerr << "ERROR: Unknown input type: " << stype << std::endl;
        exit(1);
      }
      var_map[sname] = (Var){ type, srows, scols };
    }
  }
}

void write_exo_input(Files& files, State& state) {
  // build struct
  files.exo << "struct In {" << std::endl;
  for (auto& var : state.in_vars) {
    if (var.second.type == INT) {
      files.exo << "  int " << var.first << ";" << std::endl;
    } else if (var.second.type == ARR) {
      files.exo << "  int " << var.first << "[" << var.second.rows << "];" << std::endl;
    } else if (var.second.type == MAT) {
      files.exo << "  int " << var.first << "[" << var.second.rows << " * " << var.second.cols << "];" << std::endl;
    } else if (var.second.type == CUSTOM) {
      files.exo << "  " << var.first << std::endl;
    }
  }
  files.exo << "};" << std::endl;
  // build input reader function
  files.exo << "void read_input(struct In *input) {" << std::endl;
  // initialize DEFAULT_ITER1ATOR
  files.exo << "\tint ITER1; int ITER2;" << std::endl;
  files.exo << "\tFILE *fp;" << std::endl;
  // !!! Hacking so that exo of TS reads from TE !!!
  // They should be the same file but might want to consider separating them into two EXO files
  files.exo << "\tfp = fopen(\"prover_verifier_shared/" << state.name << "_te.inputs\", \"r\");" << std::endl;
  // for all vars in the input section, read them from the file using fscanf
  for (auto var : state.in_vars) {
    if (var.second.type == INT) {
      files.exo << "\tfscanf(fp, \"%d\", &input->" << var.first << ");" << std::endl;
    } else if (var.second.type == ARR) {
      files.exo << "\tfor (ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      files.exo << "\t\tfscanf(fp, \"%d\", &input->" << var.first << "[ITER1]);" << std::endl;
      files.exo << "\t}" << std::endl;
    } else if (var.second.type == MAT) {
      files.exo << "\tfor (ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      files.exo << "\t\tfor (ITER2 = 0; ITER2 < " << var.second.cols << "; ITER2++) {" << std::endl;
      files.exo << "\t\t\tfscanf(fp, \"%d\", &input->" << var.first << "[ITER1 * " << var.second.cols << " + " << "ITER2]);" << std::endl;
      files.exo << "\t\t}" << std::endl;
      files.exo << "\t}" << std::endl;
    }
  }
  // close the file
  files.exo << "\tfclose(fp);" << std::endl;
  files.exo << "}" << std::endl;
  // add final line to start compute function
  files.exo << "void compute(struct In *input) {" << std::endl;
  // initialize DEFAULT_ITER1ATOR
  files.exo << "\tint ITER1; int ITER2;" << std::endl;
    // map input variables to local vars
    for (auto& var : state.in_vars) {
      if (var.second.type == INT) {
        files.exo << "\tint " << var.first << " = input->" << var.first << ";" << std::endl;
      }
    }
}

void write_inp_input(Files& files, State& state, std::string& name) {
  files.inp_ti << "void " << name << "_ti_input_gen (mpq_t * input_q, int num_inputs, char *argv[]) {" << std::endl;
  files.inp_te << "void " << name << "_te_input_gen (mpq_t * input_q, int num_inputs, char *argv[]) {" << std::endl;
  files.inp_ts << "void " << name << "_ts_input_gen (mpq_t * input_q, int num_inputs, char *argv[]) {" << std::endl;
}

void write_viper_input_output(std::ofstream& file, State& state) {
  // init method
  file << "method " << state.name << "(";
  // write input vars
  for (auto var : state.in_vars) {
    file << var.first << ": ";
    if (var.second.type == INT) {
      file << "Int";
    } else if (var.second.type == ARR) {
      file << "IArray";
    } else if (var.second.type == MAT) {
      file << "Seq[IArray]";
    }
    file << ", ";
  }
  // write defines as vars
  for (auto var : state.defines) {
    file << var << ": Int, ";
  }
  // remove last comma
  file.seekp(-2, std::ios_base::end);
  // complete header for method
  file << ")" << std::endl;
}

void write_pequin_input_output(std::ofstream& file, State& state, bool all_arr) {
  // build input struct and output struct
  file << "struct In {" << std::endl;
  for (auto& var : state.in_vars) {
    if (var.second.type == INT) {
      file << "  int " << var.first << (all_arr ? "[1]" : "") << ";" << std::endl;
    } else if (var.second.type == ARR) {
      file << "  int " << var.first << "[" << var.second.rows << "];" << std::endl;
    } else if (var.second.type == MAT) {
      file << "  int " << var.first << "[" << var.second.rows << " * " << var.second.cols << "];" << std::endl;
    } else if (var.second.type == CUSTOM) {
      file << "  " << var.first << std::endl;
    }
  }
  file << "};" << std::endl;
  file << "struct Out {" << std::endl;
  for (auto& var : state.out_vars) {
    if (var.second.type == INT) {
      file << "  int " << var.first << ";" << std::endl;
    } else if (var.second.type == ARR) {
      file << "  int " << var.first << "[" << var.second.rows << "];" << std::endl;
    } else if (var.second.type == MAT) {
      file << "  int " << var.first << "[" << var.second.rows << " * " << var.second.cols << "];" << std::endl;
    } else if (var.second.type == CUSTOM) {
      file << "  " << var.first << std::endl;
    }
  }
  file << "};" << std::endl;
  if (!all_arr) {
    // header for main function
    file << "void compute(struct In *input, struct Out *output) {" << std::endl;
    // initialize DEFAULT_ITER1ATOR
    file << "\tint ITER1; int ITER2;" << std::endl;
    // map input variables to local vars
    for (auto& var : state.in_vars) {
      if (var.second.type == INT) {
        file << "\tint " << var.first << " = input->" << var.first << ";" << std::endl;
      }
    }
  }
}

void compile_input(Files& files, State& state, std::string& name) {
  std::cout << "Parsing Inputs..." << std::endl;
  // parse inputs
  load_vars(files, state.in_vars, "OUT");
  // write input variables to key locations in all files
  write_exo_input(files, state);
  // Strip XXX/ in file name
  while (name.find("/") != std::string::npos) {
    name = name.substr(name.find("/") + 1, name.size());
  }
  write_inp_input(files, state, name);
}

void compile_output(Files& files, State& state) {
  std::cout << "Parsing Ouputs..." << std::endl;
  load_vars(files, state.out_vars, "REQ-TiTe");
  // write output variables to key locations in all files
  write_viper_input_output(files.vie, state);
  write_viper_input_output(files.ves, state);
  write_pequin_input_output(files.tic, state, false);
  write_pequin_input_output(files.tsc, state, true);
  write_pequin_input_output(files.tec, state, true);
}

void write_ghost_loads(std::ofstream& file, State& state) {
  // create ghost struct
  file << "typedef struct ghost_s {" << std::endl;
  file << "\tint values[";
  bool first = true;
  for (auto& var : state.ghost_vars) {
    if (!first) { file << " + "; }
    first = false;
    if (var.second.type == INT) {
      file << 1;
    } else if (var.second.type == ARR) {
      file << var.second.rows;
    } else if (var.second.type == MAT) {
      file << var.second.rows << " * " << var.second.cols;
    }
  }
  file << "];" << std::endl;
  file << "} ghost_t;" << std::endl;
  // create function header and load ghost variables to local variables using prefixes
  file << "void compute(struct In *input, struct Out *output) {" << std::endl;
  // initialize DEFAULT_ITERATOR
  file << "\tint ITER1; int ITER2;" << std::endl;
  int var_size = 0;
  for (auto& var : state.in_vars) {
    if (var.second.type != CUSTOM) {
      var_size++;
    }
  }  
  file << "\tint *public_info[" << var_size << "] = {";
  first = true;
  for (auto& var : state.in_vars) {
    if (var.second.type != CUSTOM) {
      if (!first) { file << ", "; }
      first = false;
      file << "input->" << var.first;
    }
  }
  file << "};" << std::endl;
  file << "\tghost_t ghost[1];" << std::endl;
  file << "\tint len[" << var_size << "] = {";
  first = true;
  for (auto& var : state.in_vars) {
    if (var.second.type != CUSTOM) {
      if (!first) { file << ", "; }
      first = false;
      if (var.second.type == INT) {
        file << 1;
      } else if (var.second.type == ARR) {
        file << var.second.rows;
      } else if (var.second.type == MAT) {
        file << var.second.rows << " * " << var.second.cols;
      }
    }
  }
  file << "};" << std::endl;
  file << "\texo_compute(public_info, len, ghost, 1);" << std::endl;
  // map ghost variables to local vars
  std::string count = "0";
  for (auto& var : state.ghost_vars) {
    if (var.second.type == INT) {
      if (state.uflag) {
        file << "\tuint32_t ";
      } else {
        file << "\tint ";
      }
      file << var.first << " = ghost[0].values[" << count << "];" << std::endl;
      count = count + " + 1";
    } else if (var.second.type == ARR) {
      if (state.uflag) {
        file << "\tuint32_t ";
      } else {
        file << "\tint ";
      }
      file << var.first << "[" << var.second.rows << "];" << std::endl;
      file << "\tfor (ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      file << "\t\t" << var.first << "[ITER1] = ghost[0].values[" << count << " + ITER1];" << std::endl;
      file << "\t}" << std::endl;
      count = count + " + " + var.second.rows;
    } else if (var.second.type == MAT) {
      if (state.uflag) {
        file << "\tuint32_t ";
      } else {
        file << "\tint ";
      }
      file << var.first << "[" << var.second.rows << " * " << var.second.cols << "];" << std::endl;
      file << "\tfor (ITER1 = 0; ITER1 < " << var.second.rows << " * " << var.second.cols << "; ITER1++) {" << std::endl;
      file << "\t\t" << var.first << "[ITER1] = ghost[0].values[" << count << " + ITER1];" << std::endl;
      file << "\t}" << std::endl;
      count = count + " + " + var.second.rows + " * " + var.second.cols;
    }
  }
  // map input variables to local vars (handling elevation to arrays correctly)
  for (auto& var : state.in_vars) {
    if (var.second.type == INT) {
      if (state.uflag) {
        file << "\tuint32_t ";
      } else {
        file << "\tint ";
      }
      file << var.first << " = input->" << var.first << "[0];" << std::endl;
    }
  }
}

void write_exo_emissions(std::ofstream& file, State& state) {
  // for each ghost variable, print it to console from exo code
  for (auto& var : state.ghost_vars) {
    if (var.second.type == INT) {
      file << "\tprintf(\"%d\\n\", " << var.first << ");" << std::endl;
    } else if (var.second.type == ARR) {
      file << "\tfor(ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      file << "\t\tprintf(\"%d\\n\", " << var.first << "[ITER1]);" << std::endl;
      file << "\t}" << std::endl;
    } else if (var.second.type == MAT) {
      file << "\tfor(ITER1 = 0; ITER1 < " << var.second.rows << " * " << var.second.cols << "; ITER1++) {" << std::endl;
      file << "\t\tprintf(\"%d\\n\", " << var.first << "[ITER1]);" << std::endl;
      file << "\t}" << std::endl;
    }
  }
}

void write_ghost_havocs(std::ofstream& file, State& state) {
  // for each ghost variable, add a line in the viper file to havoc it
  for (auto& var : state.ghost_vars) {
    if (var.second.type == INT) {
      file << "\tvar " << var.first << ": Int" << std::endl;
    }
    if (var.second.type == ARR) {
      file << "\tvar " << var.first << ": IArray; ";
      file << "inhale access(" << var.first << "); ";
      file << "assume len(" << var.first << ") == " << var.second.rows << std::endl;
    }
    if (var.second.type == MAT) {
      file << "\tvar " << var.first << ": Seq[IArray] := Seq();" << std::endl;
    }
  }
}

void write_output(Files& files, State& state) {
  std::cout << "Writing Pequin Output Assignments..." << std::endl;
  // for each output variable add a line to write local variable to it in all pequin c files
  for (auto& var : state.out_vars) {
    if (var.second.type == INT) {
      files.tic << "\toutput->" << var.first << " = " << var.first << ";" << std::endl;
      // files.tsc << "\toutput->" << var.first << " = " << var.first << ";" << std::endl;
      // files.tec << "\toutput->" << var.first << " = " << var.first << ";" << std::endl;
    } else if (var.second.type == ARR) {
      files.tic << "\tfor(ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      files.tic << "\t\toutput->" << var.first << "[ITER1] = " << var.first << "[ITER1];" << std::endl;
      files.tic << "\t}" << std::endl;
      // files.tsc << "\tfor(ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      // files.tsc << "\t\toutput->" << var.first << "[ITER1] = " << var.first << "[ITER1];" << std::endl;
      // files.tsc << "\t}" << std::endl;
      // files.tec << "\tfor(ITER1 = 0; ITER1 < " << var.second.rows << "; ITER1++) {" << std::endl;
      // files.tec << "\t\toutput->" << var.first << "[ITER1] = " << var.first << "[ITER1];" << std::endl;
      // files.tec << "\t}" << std::endl;
    }
    // TO DO handle matrices if needed
  }
  files.inp_ti << "}" << std::endl;
  files.inp_te << "}" << std::endl;
  files.inp_ts << "}" << std::endl;
}

#endif