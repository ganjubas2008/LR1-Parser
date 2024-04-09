#pragma once
#include "SOURCE.hpp"

struct Rule {
  Token lhs;
  Word rhs;
  string ToString() const;
};

template <> struct std::hash<Rule> {
  size_t operator()(const Rule &rule) const {
    ;
    return hash<string>()(rule.ToString());
  }
};

bool operator==(const Rule &rule_1, const Rule &rule_2);

const Rule kRuleBegin{.lhs = "Sp", .rhs = {"S"}};

class Grammar {
public:
  unordered_map<Token, vector<Word>> rules;

  vector<Rule> rules_vector;
  unordered_map<Rule, int> rule_idx;

  unordered_set<Token> alphabet;
  unordered_set<Token> non_terminals;
  unordered_set<Token> terminals;

  Grammar() = default;

  Grammar(const vector<Rule> &init_rules_vector) { // no copy
    _AddRule(kRuleBegin);
    for (const Rule &rule : init_rules_vector) {
      _AddRule(rule);
    }
  }

  bool IsNonTerm(const Token &symbol) const;

  bool IsTerm(const Token &symbol) const;

protected:
  void _AddRule(const Rule &rule);
};

class LRGrammar : public Grammar {
public:
  LRGrammar() = default;

  LRGrammar(const vector<Rule> &init_rules_vector) { // no copy
    _AddRule(kRuleBegin);
    for (const Rule &rule : init_rules_vector) {
      _AddRule(rule);
    }
  }

  void PrecalcFirstToken();

  set<Token> GetFirst(const Word &) const;

  // void _DbgFirst() const {
  //   for (const auto& non_term: non_terminals) {
  //     cout << non_term << ": ";
  //     for (const auto& token: GetFirst({non_term})) {
  //       cout << token << ", ";
  //     }
  //     cout << "\n";
  //   }
  //   cout << "\n____________________________________\n";
  // }

private:
  mutable unordered_map<Token, bool> _is_eps_generating;
  mutable unordered_map<Token, set<Token>> _memo_first_token;
  mutable unordered_map<Word, set<Token>> _memo_first_word;

  void _PrecalcEpsGeneratingToken();

  bool _RelaxToken(const Token &, const Word &);
};
