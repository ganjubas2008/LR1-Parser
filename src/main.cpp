#include <getopt.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "parser.hpp"
#include "reader.hpp"

class ArgumentParser {
public:
  string grammar_file;
  string word_file;

  void ParseArguments(int argc, char **argv) {
    int opt;
    int option_index;

    while ((opt = getopt_long(argc, argv, "g:w:", kLongOptions,
                              &option_index)) != -1) {
      switch (opt) {
      case 'g':
        grammar_file = optarg;
        break;
      case 'w':
        word_file = optarg;
        break;
      default:
        PrintUsage(cerr);
        throw invalid_argument("");
      }
    }
    ValidateArguments(grammar_file, word_file);
  }

private:
  static constexpr option kLongOptions[] = {
      {"grammar-file", required_argument, NULL, 'g'},
      {"word-file", required_argument, NULL, 'w'},
      {0, 0, 0}};

  static void PrintUsage(ostream &os) {
    static const string kUsage = "Usage: run [-g <GRAMMAR_FILE>]";
    os << kUsage << endl;
  }

  static void ValidateArguments(const string &grammar_file,
                                const string &word_file) {
    if (grammar_file.empty()) {
      PrintUsage(cerr);
      throw invalid_argument("Grammar file is not given");
    }
    if (word_file.empty()) {
      PrintUsage(cerr);
      throw invalid_argument("Word file is not given");
    }
  }
};

int main(int argc, char *argv[]) {
  ArgumentParser argparser;
  argparser.ParseArguments(argc, argv);

  LRGrammar grammar = GrammarReader().Read(argparser.grammar_file);
  Word word = WordReader().Read(argparser.word_file);

  LRParser parser(grammar);
  parser.BuildTable();

  cout << (parser.Predict(word) ? "TRUE" : "FALSE");

  return 0;
}
