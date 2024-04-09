#pragma once
#pragma once
#include "grammar.hpp"

struct Reader {
  static vector<string> SplitString(const string &input,
                                              const string &delimiter);

  static string TrimString(const string &input);
};

struct GrammarReader : Reader {
  static vector<Rule> ReadRules(const string &input);

  LRGrammar Read(const string &filename);
};

struct TestWord {
  Word word;
  bool ans;
};

struct WordReader : Reader {
  Word Read(const string &filename);
};

struct TestWordsReader : Reader {
  static TestWord ReadTestWord(const string &input);

  vector<TestWord> Read(const string &filename);
};