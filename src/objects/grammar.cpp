#include "grammar.hpp"

string Rule::ToString() const {
  string str = kLeftBound + lhs + kRightBound + kImplies;
  for (const auto &token : rhs) {
    str += kLeftBound + token + kRightBound;
  }
  return str;
}

bool operator==(const Rule &rule_1, const Rule &rule_2) {
  return (rule_1.lhs == rule_2.lhs && rule_1.rhs == rule_2.rhs);
}

bool Grammar::IsNonTerm(const Token &symbol) const {
  return non_terminals.find(symbol) != non_terminals.end();
}

bool Grammar::IsTerm(const Token &symbol) const {
  return terminals.find(symbol) != terminals.end();
}

void Grammar::_AddRule(const Rule &rule) {
  if (rule_idx.find(rule) != rule_idx.end()) {
    return;
  }
  rule_idx[rule] = rule_idx.size();
  for (const Token &token : rule.rhs) {
    if (token != "") {
      terminals.insert(token);
      alphabet.insert(token);
    }
  }
  non_terminals.insert(rule.lhs);
  alphabet.insert(rule.lhs);
  rules_vector.push_back(rule);
  rules[rule.lhs].push_back(rule.rhs);
  for (const Token &non_term : non_terminals) {
    terminals.erase(non_term);
  }
}

void LRGrammar::_PrecalcEpsGeneratingToken() {
  /*
  Предподсчёт токенов, из которых выводится пустое слово
  */
  _is_eps_generating[""] = true;
  bool changes = true;
  while (changes) {
    changes = false;
    for (const Token &token : non_terminals) {
      for (const Word &word : rules[token]) {
        bool all_eps_gen = true;
        for (const Token &token : word) {
          all_eps_gen &= _is_eps_generating[{token}];
        }
        changes |= (!_is_eps_generating[token] && all_eps_gen);
        _is_eps_generating[token] |= all_eps_gen;
      }
    }
  }
}

bool LRGrammar::_RelaxToken(const Token &token, const Word &word) {
  /*
  Хотим обновить символы выводимые из токена, через символы выводимые из слова
  (token -> word -> aX) => (a \in _memo_first_token[token])
  Возвращаем bool - произошли ли какие-то изменения
  */
  bool changes = false;
  size_t i = 0;
  changes |= InsertSet(_memo_first_token[token], _memo_first_token[word[0]]);
  while (i + 1 < word.size() && _is_eps_generating[word[i]]) {
    changes |=
        InsertSet(_memo_first_token[token], _memo_first_token[word[i + 1]]);
    ++i;
  }
  return changes;
}

void LRGrammar::PrecalcFirstToken() {
  /*
  Преподсчёт First[token]
  */
  _PrecalcEpsGeneratingToken();
  for (const Token &term : terminals) {
    _memo_first_token[term].insert(term);
  }
  bool changes = true;
  while (changes) {
    changes = false;
    for (const Token &non_term : non_terminals) {
      for (const Word &word : rules[non_term]) {
        changes |= _RelaxToken(non_term, word);
      }
    }
  }
  _memo_first_word[{}] = {"$"};
}

set<Token> LRGrammar::GetFirst(const Word &word) const {
  if (!_memo_first_word[word].empty()) {
    return _memo_first_word[word];
  }
  size_t i = 0;
  InsertSet(_memo_first_word[word], _memo_first_token.at(word[0]));
  while (i + 1 < word.size() && _is_eps_generating.at(word[i])) {
    InsertSet(_memo_first_word[word], _memo_first_token.at(word[i + 1]));
    ++i;
  }
  if (i + 1 == word.size() && _is_eps_generating.at(word[i])) {
    _memo_first_word[word].insert("$");
  }
  return _memo_first_word[word];
}
