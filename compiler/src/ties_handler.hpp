
#ifndef TIES_HANDLER_H
#define TIES_HANDLER_H

#include "file_utils.hpp"
#include "compiler_state.hpp"
#include <vector>
#include <string>
#include <ctype.h>
#include <math.h>

bool isWhitespace(std::string s){
    for(int index = 0; index < s.length(); index++){
        if(!std::isspace(s[index]))
            return false;
    }
    return true;
}

// remove .val from end of token (for arrays in c)
std::string strip_dotval(std::string& str) {
  if (str.find(".val") != std::string::npos) {
    return str.substr(0, str.find(".val"));
  } else {
    return str;
  }
}
// remove *-> from a token (for viper)
std::string strip_arrow(const std::string& str) {
  if (str.find("->") != std::string::npos) {
    return str.substr(str.find("->") + 2, str.size());
  } else {
    return str;
  }
}
// Detect if "==" , "!=", "<", "<=", ">", ">=" ever appears in a sequence of strings
// If not, we need to append "!= 0" at the end of the assertion
bool is_comparison(std::vector<std::string>& tokens) {
  for (int i = 0; i < tokens.size(); i++) {
    if (tokens[i] == "==" || tokens[i] == "!=" || tokens[i] == "<" || tokens[i] == "<=" || tokens[i] == ">" || tokens[i] == ">=")
      return true;
  }
  return false;
}

std::vector<std::string> tokens_viper_preprocess(std::vector<std::string>& old_tokens) {
  std::vector<std::string> new_tokens;
  // How many iterations after this do we want to skip?
  int skip = 0;
  for (int i = 0; i < old_tokens.size(); i++) {
    if (skip > 0) {
      skip--;
    } else {
      if (old_tokens[i] == ">>") {
        // Convert right shifts to division
        new_tokens.push_back("/");
        new_tokens.push_back(std::to_string((int) pow(2, stoi(old_tokens[i + 1]))));
        skip = 1;
      } else if (old_tokens[i] == "mat_slot(") {
        // Convert matrix to list + IArray
        new_tokens.push_back("slot(");
        new_tokens.push_back(old_tokens[i + 1].substr(0, old_tokens[i + 1].size() - 1) + "[" + old_tokens[i + 3].substr(0, old_tokens[i + 3].size() - 1) + "],");
        skip = 3;
      } else {
        new_tokens.push_back(old_tokens[i]);
      }
    }
  }

  std::vector<std::string> no_arr_tokens;
  for (int i = 0; i < new_tokens.size(); i++) {
    no_arr_tokens.push_back(strip_arrow(new_tokens[i]));
  }
  return no_arr_tokens;
}

// TI tokens are, %, int, loop, if, else, end, assume
void compile_Ti(Files& files, State& state) {
  // If exo_only is set to true, then lines will only be written to vie and exo
  bool exo_only = false;
  std::cout << "Compiling T_I..." << std::endl;
  // init state
  state.prev_token_zero = "";
  state.indent = 1;
  state.loop_vars = {};
  state.in_loop_invariant = false;
  // read the T_I section line by line
  std::string line;
  while (std::getline(files.src, line)) {
    // if we hit next section then exit
    if (line == "TE") { break; }
    // otherwise if the line is not empty then we break it into tokens and handle it based on the first token
    else if (!isWhitespace(line)) {
      // split line into tokens by spaces
      std::vector<std::string> tokens = tokenize(line);
      // Setup EXO only line
      exo_only = false;
      if (tokens[0] == "%exo") {
        exo_only = true;
        for (int i = 0; i < tokens.size() - 1; i++) {
          tokens[i] = tokens[i + 1];
        }
        tokens.pop_back();
      }
      std::vector<std::string> v_tokens = tokens_viper_preprocess(tokens);
      // if previous token zero was a % or loop and current token zero is not a % then add a '{' to vie
      if (state.in_loop_invariant && tokens[0].front() != '%') {
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
        files.vie << "{" << std::endl;
        state.indent++;
        state.in_loop_invariant = false;
      }
      // if current token[0] is "else" or "end" then artificially deindent
      if (tokens[0] == "else" || tokens[0] == "end" || tokens[0] == "endloop") { state.indent--; }
      // indent line based on current indent level
      for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
      if(tokens[0].front() != '%' && tokens[0] != "assume") {
        if (!exo_only) { for (int i = 0; i < state.indent; i++) { files.tic << "\t"; } }
        for (int i = 0; i < state.indent; i++) { files.exo << "\t"; }
      }
      // based on token 0 we handle the line
      if(tokens[0] == "%") {
        // if the token is a %, strip the % and emit as is to both viper files
        for (int i = 1; i < tokens.size(); i++) {
          files.vie << tokens[i] << " ";
        }
        files.vie << std::endl;
      } else if (tokens[0] == "%c") {
        // if the token is a %c, strip the %c and emit as is to all relevant c files
        for (int i = 1; i < tokens.size(); i++) {
          if (!exo_only) { files.tic << tokens[i] << " "; }
          files.exo << tokens[i] << " ";
        }
        if (!exo_only) { files.tic << std::endl; }
        files.exo << std::endl;
      } else if (tokens[0] == "%vie") {
        // if the token is a %vie, strip the %vie and emit as is to the viper vie file
        for (int i = 1; i < tokens.size(); i++) {
          files.vie << tokens[i] << " ";
        }
        files.vie << std::endl;
      } else if (tokens[0] == "buffet") {
        if (!exo_only) { files.tic << "[[buffet::fsm(" << tokens[1] << ")]]" << std::endl; }
      } else if (tokens[0] == "int" || tokens[0] == "ghostInt") {
        // add variable to state ghost vars if ghost
        if (tokens[0] == "ghostInt") {
          Type type = INT;
          state.ghost_vars[tokens[1]] = (Var){ type, "", "" };
        }
        // handle vie
        files.vie << "var " << v_tokens[1] << ": Int := ";
        for (int i = 3; i < v_tokens.size(); i++) {
          files.vie << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
        }
        files.vie << std::endl;
        // handle tic and exo
        if (!exo_only) { files.tic << "int " << tokens[1] << " = "; }
        files.exo << "int " << tokens[1] << " = ";
        for (int i = 3; i < tokens.size(); i++) {
          if (!exo_only) { files.tic << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " "); }
          files.exo << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        if (!exo_only) { files.tic << ";" << std::endl; }
        files.exo << ";" << std::endl;
      } else if (tokens[0] == "arr" || tokens[0] == "ghostArr") {
        // add ghost arrays
        if (tokens[0] == "ghostArr") {
          Type type = ARR;
          state.ghost_vars[tokens[1]] = (Var){ type, tokens[2], "" };
        }
        // handle vie
        files.vie << "var " << tokens[1] << ": IArray; ";
        files.vie << "inhale access(" << tokens[1] << "); ";
        files.vie << "assume len(" << tokens[1] << ") == " << tokens[2] << std::endl;
        // handle tic and exo
        if (!exo_only) { files.tic << "int " << tokens[1] << "[" << tokens[2] << "];" << std::endl; }
        files.exo << "int " << tokens[1] << "[" << tokens[2] << "];" << std::endl;
      } else if (tokens[0] == "mat" || tokens[0] == "ghostMat") {
        // add ghost arrays
        if (tokens[0] == "ghostMat") {
          Type type = MAT;
          state.ghost_vars[tokens[1]] = (Var){ type, tokens[2], tokens[3] };
        }
        // handle vie
        files.vie << "var " << tokens[1] << ": Seq[IArray] := Seq()" << std::endl;
        // handle tic and exo
        if (!exo_only) { files.tic << "int " << tokens[1] << "[" << tokens[2] << " * " << tokens[3] << "];" << std::endl; }
        files.exo << "int " << tokens[1] << "[" << tokens[2] << " * " << tokens[3] << "];" << std::endl;
      } else if (tokens[0] == "loop") {
        // handle vie
        files.vie << "var " << tokens[1] << ": Int" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
        files.vie << tokens[1] << " := 0" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
        files.vie << "while(" << tokens[1] << " < " << tokens[2] << ")" << std::endl;
        // handle tic
        if (!exo_only) { files.tic << "int " << tokens[1] << "; for(" << tokens[1] << " = 0; " << tokens[1] << " < " << tokens[2] << "; " << tokens[1] << "++){" << std::endl; }
        // handle exo
        files.exo << "int " << tokens[1] << "; for(" << tokens[1] << " = 0; " << tokens[1] << " < " << tokens[2] << "; " << tokens[1] << "++){" << std::endl;
        // add loop var to state loop vars
        state.loop_vars.push_back(tokens[1]);
        state.in_loop_invariant = true;
      } else if (tokens[0] == "while") {
        files.vie << "while(";
        if (!exo_only) { files.tic << "while("; }
        files.exo << "while(";
        for (int i = 1; i < tokens.size(); i++) {
          files.vie << strip_arrow(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
          if (!exo_only) { files.tic << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " "); }
          files.exo << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.vie << ")" << std::endl;
        if (!exo_only) { files.tic << "){" << std::endl; }
        files.exo << "){" << std::endl;
        state.in_loop_invariant = true;
      } else if (tokens[0] == "if") {
        // handle vie, tic, and exo
        files.vie << "if(";
        if (!exo_only) { files.tic << "if("; }
        files.exo << "if(";
        for (int i = 1; i < v_tokens.size(); i++) {
          files.vie << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
        }
        for (int i = 1; i < tokens.size(); i++) {
          if (!exo_only) { files.tic << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " "); }
          files.exo << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.vie << ") {" << std::endl;
        if (!exo_only) { files.tic << ") {" << std::endl; }
        files.exo << ") {" << std::endl;
        // indent for next line
        state.indent++;
      } else if (tokens[0] == "else") {
        // print "} else {" to all relevant files
        files.vie << "} else {" << std::endl;
        if (!exo_only) { files.tic << "} else {" << std::endl; }
        files.exo << "} else {" << std::endl;
        // indent for next line
        state.indent++;
      } else if (tokens[0] == "end" || tokens[0] == "endloop") {
        // if endloop then write increment in viper and pop loop var from state loop vars
        if (tokens[0] == "endloop") {
          files.vie << "\t" << state.loop_vars.back() << " := " << state.loop_vars.back() << " + 1" << std::endl;
          for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
          state.loop_vars.pop_back();
        }
        // emit a '}' to all relevant files
        files.vie << "}" << std::endl;
        if (!exo_only) { files.tic << "}" << std::endl; }
        files.exo << "}" << std::endl;
      } else if (tokens[0] == "assume") {
        // handle vie
        for (int i = 0; i < v_tokens.size(); i++) {
          files.vie << v_tokens[i] << (i + 1 == tokens.size() ? "" : " ");
        }
        files.vie << std::endl;
        // TO DO handle tic
      } else { // no error checking, assume this is an assignment line
        for (int i = 0; i < v_tokens.size(); i++) {
          files.vie << (v_tokens[i] == "=" ? ":=" : v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
        }
        for (int i = 0; i < tokens.size(); i++) {
          if (!exo_only) { files.tic << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " "); }
          files.exo << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.vie << std::endl;
        if (!exo_only) { files.tic << ";" << std::endl; }
        files.exo << ";" << std::endl;
      }
      state.prev_token_zero = tokens[0];
    }
  }
}

void compile_Te(Files& files, State& state, bool aFlag) {
  std::cout << "Compiling T_E..." << std::endl;
  // init state
  state.prev_token_zero = "";
  state.indent = 1;
  state.loop_vars = {};
  state.in_loop_invariant = false;
  // init accumErr [disable to get proper paper constraint count]
  if (aFlag) { files.tec << "\tint accumErr = 0;" << std::endl; }
  // read the T_I section line by line
  std::string line;
  while (std::getline(files.src, line)) {
    // if we hit next section then exit
    if (line == "TS") { break; }
    // otherwise if the line is not empty then we break it into tokens and handle it based on the first token
    else if (!isWhitespace(line)) {
      // split line into tokens by spaces
      std::vector<std::string> tokens = tokenize(line);
      std::vector<std::string> v_tokens = tokens_viper_preprocess(tokens);
      // if previous token zero was a % or loop and current token zero is not a % then add a '{' to vie and ves
      if (state.in_loop_invariant && tokens[0].front() != '%') {
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
        files.vie << "{" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
        files.ves << "{" << std::endl;
        state.indent++;
        state.in_loop_invariant = false;
      }
      // if current token[0] is "else" or "end" then artificially deindent
      if (tokens[0] == "else" || tokens[0] == "end" || tokens[0] == "endloop") { state.indent--; }
      // indent line based on current indent level
      if (tokens[0] != "%ves") {
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
      }
      if (tokens[0] != "%vie") {
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
      }
      if(tokens[0].front() != '%') {
        for (int i = 0; i < state.indent; i++) { files.tec << "\t"; }
      }
      // based on token 0 we handle the line
      if(tokens[0] == "%") {
        // if the token is a %, strip the % and emit as is to both viper files
        for (int i = 1; i < tokens.size(); i++) {
          files.vie << tokens[i] << " ";
          files.ves << tokens[i] << " ";
        }
        files.vie << std::endl;
        files.ves << std::endl;
      } else if (tokens[0] == "%c") {
        // if the token is a %c, strip the %c and emit as is to all relevant c files
        for (int i = 1; i < tokens.size(); i++) {
          files.tec << tokens[i] << " ";
        }
        files.tec << std::endl;
      } else if (tokens[0] == "%vie") {
        // if the token is a %vie, strip the %vie and emit as is to the viper vie file
        for (int i = 1; i < tokens.size(); i++) {
          files.vie << tokens[i] << " ";
        }
        files.vie << std::endl;
      } else if (tokens[0] == "%ves") {
        // if the token is a %ves, strip the %ves and emit as is to the viper ves file
        for (int i = 1; i < tokens.size(); i++) {
          files.ves << tokens[i] << " ";
        }
        files.ves << std::endl;
      } else if (tokens[0] == "buffet") {
        files.tec << "[[buffet::fsm(" << tokens[1] << ")]]" << std::endl;
      } else if (tokens[0] == "int") {
        // TO DO, later we need to handle case where var is declated in T_I before T_E
        // handle vie and ves
        files.vie << "var " << tokens[1] << ": Int := ";
        files.ves << "var " << tokens[1] << ": Int := ";
        for (int i = 3; i < v_tokens.size(); i++) {
          files.vie << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
          files.ves << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
        }
        files.vie << std::endl;
        files.ves << std::endl;
        // handle tec
        if (state.uflag) {
          files.tec << "uint32_t ";
        } else {
          files.tec << "int ";
        }
        files.tec << tokens[1] << " = ";
        for (int i = 3; i < tokens.size(); i++) {
          files.tec << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.tec << ";" << std::endl;
      } else if (tokens[0] == "arr") {
        // handle vie
        files.vie << "var " << tokens[1] << ": IArray; ";
        files.vie << "inhale access(" << tokens[1] << "); ";
        files.vie << "assume len(" << tokens[1] << ") == " << tokens[2] << std::endl;
        // handle ves
        files.ves << "var " << tokens[1] << ": IArray; ";
        files.ves << "inhale access(" << tokens[1] << "); ";
        files.ves << "assume len(" << tokens[1] << ") == " << tokens[2] << std::endl;
        // handle tec and exo
        if (state.uflag) {
          files.tec << "uint32_t ";
        } else {
          files.tec << "int ";
        }
        files.tec << tokens[1] << "[" << tokens[2] << "];" << std::endl;
      } else if (tokens[0] == "loop") {
        // handle vie and ves
        files.vie << "var " << tokens[1] << ": Int" << std::endl;
        files.ves << "var " << tokens[1] << ": Int" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
        files.vie << tokens[1] << " := 0" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
        files.ves << tokens[1] << " := 0" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.vie << "\t"; }
        files.vie << "while(" << tokens[1] << " < " << tokens[2] << ")" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
        files.ves << "while(" << tokens[1] << " < " << tokens[2] << ")" << std::endl;
        // handle tec
        files.tec << "int " << tokens[1] << "; for(" << tokens[1] << " = 0; " << tokens[1] << " < " << tokens[2] << "; " << tokens[1] << "++) {" << std::endl;
        // add loop var to state loop vars
        state.loop_vars.push_back(tokens[1]);
        state.in_loop_invariant = true;
      } else if (tokens[0] == "if") {
        // handle vie and ves
        files.vie << "if(";
        files.ves << "if(";
        for (int i = 1; i < v_tokens.size(); i++) {
          files.vie << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
          files.ves << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
        }
        files.vie << ") {" << std::endl;
        files.ves << ") {" << std::endl;
        // handle tec
        files.tec << "if(";
        for (int i = 1; i < tokens.size(); i++) {
          files.tec << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.tec << ") {" << std::endl;
        // increase indent
        state.indent++;
      } else if (tokens[0] == "else") {
        // handle vie, ves, and tsc
        files.vie << "} else {" << std::endl;
        files.ves << "} else {" << std::endl;
        files.tec << "} else {" << std::endl;
        // increase indent
        state.indent++;
      } else if (tokens[0] == "end" || tokens[0] == "endloop") {
        // if endloop then write increment in viper and pop loop var from state loop vars
        if (tokens[0] == "endloop") {
          files.vie << "\t" << state.loop_vars.back() << " := " << state.loop_vars.back() << " + 1" << std::endl;
          files.ves << "\t" << state.loop_vars.back() << " := " << state.loop_vars.back() << " + 1" << std::endl;
          for (int i = 0; i < state.indent; i++) { files.vie << "\t"; files.ves << "\t"; }
          state.loop_vars.pop_back();
        }
        // handle vie, ves, and tec
        files.vie << "}" << std::endl;
        files.ves << "}" << std::endl;
        files.tec << "}" << std::endl;
      } else if (tokens[0] == "assert_zero") {
        // handle vie and ves
        files.vie << "assert !(";
        files.ves << "assume !(";
        for (int i = 1; i < v_tokens.size(); i++) {
          files.vie << strip_arrow(v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
          files.ves << strip_arrow(v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
        }
        if (!is_comparison(v_tokens)) {
          files.vie << " != 0";
          files.ves << " != 0";
        }
        files.vie << ")" << std::endl;
        files.ves << ")" << std::endl;
        // handle tec
        // AccumErr disabled for now for compatibility with old tests
        if (aFlag) {
          files.tec << "if(";
          for (int i = 1; i < tokens.size(); i++) {
            files.tec << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
          }
          if (!is_comparison(v_tokens)) {
            files.tec << " != 0";
          }
          files.tec << ") { accumErr++; }" << std::endl;
        } else {
          files.tec << "assert_zero(";
          for (int i = 1; i < tokens.size(); i++) {
            files.tec << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
          }
          files.tec << ");" << std::endl;
        }
      } else { // no error checking, assume this is an assignment line
        // handle vie and ves
        for (int i = 0; i < v_tokens.size(); i++) {
          files.vie << (v_tokens[i] == "=" ? ":=" : v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
          files.ves << (v_tokens[i] == "=" ? ":=" : v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
        }
        files.vie << std::endl;
        files.ves << std::endl;
        // handle tec
        for (int i = 0; i < tokens.size(); i++) {
          files.tec << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.tec << ";" << std::endl;
      }
      state.prev_token_zero = tokens[0];
    }
  }
  // handle final accumErr check [disable for paper constraint count]
  if (aFlag) { files.tec << "\tassert_zero(accumErr);" << std::endl; }
}

void compile_Ts(Files& files, State& state, bool aFlag) {
  std::cout << "Compiling T_S..." << std::endl;
  // init state
  state.prev_token_zero = "";
  state.indent = 1;
  state.loop_vars = {};
  state.in_loop_invariant = false;
  // init accumErr [disable to get proper paper constraint count]
  if (aFlag) { files.tsc << "\tint accumErr = 0;" << std::endl; }
  // read the T_S section line by line
  std::string line;
  while (std::getline(files.src, line)) {
    // if we hit next section then exit
    if (line == "INP") { break; }
    // otherwise if the line is not empty then we break it into tokens and handle it based on the first token
    else if (!isWhitespace(line)) {
      // split line into tokens by spaces
      std::vector<std::string> tokens = tokenize(line);
      std::vector<std::string> v_tokens = tokens_viper_preprocess(tokens);
      // if previous token zero was a % or loop and current token zero is not a % then add a '{' to ves
      if (state.in_loop_invariant && tokens[0].front() != '%') {
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
        files.ves << "{" << std::endl;
        state.indent++;
        state.in_loop_invariant = false;
      }
      // if current token[0] is "else" or "end" then artificially deindent
      if (tokens[0] == "else" || tokens[0] == "end" || tokens[0] == "endloop") { state.indent--; }
      // indent line based on current indent level
      for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
      if(tokens[0].front() != '%' && tokens[0] != "assert") {
        for (int i = 0; i < state.indent; i++) { files.tsc << "\t"; }
      }
      // based on token 0 we handle the line
      if(tokens[0] == "%") {
        // if the token is a %, strip the % and emit as is to viper ves file
        for (int i = 1; i < tokens.size(); i++) {
          files.ves << tokens[i] << " ";
        }
        files.ves << std::endl;
      } else if (tokens[0] == "%c") {
        // if the token is a %c, strip the %c and emit as is to all relevant c files
        for (int i = 1; i < tokens.size(); i++) {
          files.tsc << tokens[i] << " ";
        }
        files.tsc << std::endl;
      } else if (tokens[0] == "%ves") {
        // if the token is a %ves, strip the %ves and emit as is to the viper ves file
        for (int i = 1; i < tokens.size(); i++) {
          files.ves << tokens[i] << " ";
        }
        files.ves << std::endl;
      } else if (tokens[0] == "buffet") {
        files.tsc << "[[buffet::fsm(" << tokens[1] << ")]]" << std::endl;
      } else if (tokens[0] == "int") {
        // handle ves
        files.ves << "var " << tokens[1] << ": Int := ";
        for (int i = 3; i < v_tokens.size(); i++) {
          files.ves << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
        }
        files.ves << std::endl;
        // handle tsc
        if (state.uflag) {
          files.tsc << "uint32_t ";
        } else {
          files.tsc << "int ";
        }
        files.tsc << tokens[1] << " = ";
        for (int i = 3; i < tokens.size(); i++) {
          files.tsc << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.tsc << ";" << std::endl;
      } else if (tokens[0] == "arr") {
        // handle ves
        files.ves << "var " << tokens[1] << ": IArray; ";
        files.ves << "inhale access(" << tokens[1] << "); ";
        files.ves << "assume len(" << tokens[1] << ") == " << tokens[2] << std::endl;
        // handle tsc
        if (state.uflag) {
          files.tsc << "uint32_t ";
        } else {
          files.tsc << "int ";
        }
        files.tsc << tokens[1] << "[" << tokens[2] << "];" << std::endl;
      } else if (tokens[0] == "havoc") {
        // handle ves
        files.ves << "var " << tokens[1] << ": Int" << std::endl;
        // handle tsc
        files.tsc << "int " << tokens[1] << ";" << std::endl;
      } else if (tokens[0] == "loop") {
        // handle ves
        files.ves << "var " << tokens[1] << ": Int" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
        files.ves << tokens[1] << " := 0" << std::endl;
        for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
        files.ves << "while(" << tokens[1] << " < " << tokens[2] << ")" << std::endl;
        // handle tsc
        if (state.uflag) {
          files.tsc << "uint32_t ";
        } else {
          files.tsc << "int ";
        }
        files.tsc << tokens[1] << "; for(" << tokens[1] << " = 0; " << tokens[1] << " < " << tokens[2] << "; " << tokens[1] << "++){" << std::endl;
        // add loop var to state loop vars
        state.loop_vars.push_back(tokens[1]);
        state.in_loop_invariant = true;
      } else if (tokens[0] == "if") {
        // handle ves, tsc
        files.ves << "if(";
        files.tsc << "if(";
        for (int i = 1; i < v_tokens.size(); i++) {
          files.ves << v_tokens[i] << (i + 1 == v_tokens.size() ? "" : " ");
        }
        for (int i = 1; i < tokens.size(); i++) {
          files.tsc << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.ves << ") {" << std::endl;
        files.tsc << ") {" << std::endl;
        // indent for next line
        state.indent++;
      } else if (tokens[0] == "else") {
        // print "} else {" to all relevant files
        files.ves << "} else {" << std::endl;
        files.tsc << "} else {" << std::endl;
        // indent for next line
        state.indent++;
      } else if (tokens[0] == "end" || tokens[0] == "endloop") {
        // if endloop then write increment in viper and pop loop var from state loop vars
        if (tokens[0] == "endloop") {
          files.ves << "\t" << state.loop_vars.back() << " := " << state.loop_vars.back() << " + 1" << std::endl;
          for (int i = 0; i < state.indent; i++) { files.ves << "\t"; }
          state.loop_vars.pop_back();
        }
        // emit a '}' to all relevant files
        files.ves << "}" << std::endl;
        files.tsc << "}" << std::endl;
      } else if (tokens[0] == "assert_zero") {
        // handle ves
        files.ves << "assert !(";
        for (int i = 1; i < v_tokens.size(); i++) {
          files.ves << strip_arrow(v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
        }
        if (!is_comparison(v_tokens)) {
          files.ves << " != 0";
        }
        files.ves << ")" << std::endl;
        // handle tsc
        if (aFlag) {
          files.tsc << "if(";
          for (int i = 1; i < tokens.size(); i++) {
            files.tsc << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
          }
          if (!is_comparison(v_tokens)) {
            files.tsc << " != 0";
          }
          files.tsc << ") { accumErr++; }" << std::endl;
        } else {
          files.tsc << "assert_zero(";
          for (int i = 1; i < tokens.size(); i++) {
            files.tsc << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
          }
          files.tsc << ");" << std::endl;
        }
      } else { // no error checking, assume this is an assignment line
        for (int i = 0; i < v_tokens.size(); i++) {
          files.ves << (v_tokens[i] == "=" ? ":=" : v_tokens[i]) << (i + 1 == v_tokens.size() ? "" : " ");
        }
        for (int i = 0; i < tokens.size(); i++) {
          files.tsc << strip_dotval(tokens[i]) << (i + 1 == tokens.size() ? "" : " ");
        }
        files.ves << std::endl;
        files.tsc << ";" << std::endl;
      }
      state.prev_token_zero = tokens[0];
    }
  }
  // handle final accumErr check [disable for paper constraint count]
  if (aFlag) { files.tsc << "\tassert_zero(accumErr);" << std::endl; }
}

#endif
