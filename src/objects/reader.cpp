#include "reader.hpp"

vector<string> Reader::SplitString(const string &input,
                                             const string &delimiter) {
  vector<string> tokens;
  size_t start = 0, end = 0;
  while ((end = input.find(delimiter, start)) != string::npos) {
    if (end != start) {
      tokens.push_back(input.substr(start, end - start));
    } else {
      tokens.push_back("");
    }
    start = end + delimiter.length();
  }
  if (start != input.length()) {
    tokens.push_back(input.substr(start));
  } else {
    tokens.push_back("");
  }
  return tokens;
}

string Reader::TrimString(const string &input) {
  size_t start = input.find_first_not_of(" \t\n\r\f\v" + kLeftBound);
  size_t end = input.find_last_not_of(" \t\n\r\f\v" + kRightBound);
  if (start == string::npos) {
    return ""; // Input string contains only empty characters
  }
  return input.substr(start, end - start + 1);
}

vector<Rule> GrammarReader::ReadRules(const string &input) {
  vector<Rule> rules;
  vector<string> lhsrhss = SplitString(input, kImplies);
  Token lhs = TrimString(lhsrhss[0]);
  string rhss = lhsrhss[1];
  for (const auto &word : SplitString(rhss, kDelimiter)) {
    auto trimmed_word = TrimString(word);
    Word rhs = SplitString(trimmed_word, kRightBound + kLeftBound);
    rules.push_back(Rule(lhs, rhs));
  }
  return rules;
}

LRGrammar GrammarReader::Read(const string &filename) {
  ifstream myfile(filename);
  string input_line;

  vector<Rule> rules;
  if (myfile.is_open()) {
    while (myfile.good()) {
      getline(myfile, input_line);
      if (!input_line.empty()) {
        for (const auto &rule : ReadRules(input_line)) {
          rules.push_back(rule);
        }
      }
    }
  } else {
    throw invalid_argument("Invalid file\n");
  }
  return LRGrammar(rules);
}

Word WordReader::Read(const string &filename) {
  ifstream myfile(filename);
  if (!myfile.is_open()) {
    throw invalid_argument("Invalid file\n");
  }
  string input_line;
  getline(myfile, input_line);
  return SplitString(TrimString(input_line), kRightBound + kLeftBound);
}

TestWord TestWordsReader::ReadTestWord(const string &input) {
  vector<string> lhsrhs = SplitString(input, kImplies);
  Word lhs = SplitString(TrimString(lhsrhs[0]), kRightBound + kLeftBound);
  Token rhs = TrimString(lhsrhs[1]);
  return TestWord{.word = lhs, .ans = static_cast<bool>(rhs[0] - '0')};
}

vector<TestWord> TestWordsReader::Read(const string &filename) {
  vector<TestWord> testwords;
  ifstream myfile(filename);
  string input_line;
  if (myfile.is_open()) {
    while (myfile.good()) {
      getline(myfile, input_line);
      if (!input_line.empty()) {
        testwords.push_back(ReadTestWord(input_line));
      }
    }
  } else {
    throw invalid_argument("Invalid file\n");
  }
  return testwords;
}
