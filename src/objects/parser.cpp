#include "parser.hpp"

void LRParser::BuildTable() {
  bool IsGrammarLR1 = table.BuildTable();
  if (!IsGrammarLR1) {
    throw invalid_argument("Grammar is not LR-1");
  }
}

void LRParser::Shift(const Action &todo, const Token &token) {
  token_stack.push(token);
  int_stack.push(todo.to);
  ++word_idx;
  cur_state = todo.to;
}

bool LRParser::Reduce(const Action &todo) {
  size_t rule_idx = todo.to;
  Rule rule = grammar.rules_vector[rule_idx];
  for (size_t i = 0; i < rule.rhs.size(); ++i) {
    if (rule.rhs[rule.rhs.size() - i - 1] != token_stack.top()) {
      return false;
    }
    int_stack.pop();
    token_stack.pop();
  }
  token_stack.push(rule.lhs);
  cur_state = table.Get(int_stack.top(), rule.lhs).to;
  int_stack.push(cur_state);
  return true;
}

bool LRParser::Predict(Word word) {
  cur_state = 1;
  word_idx = 0;

  int_stack = stack<int>({cur_state});
  token_stack = stack<Token>();

  word.push_back("$");
  Action todo = table.Get(cur_state, word[word_idx]);
  while (todo != Action{Action::Reduce, 0}) {
    switch (todo.type) {
    case Action::None:
      return false;
    case Action::Goto:
      assert(false);
    case (Action::Shift):
      Shift(todo, word[word_idx]);
      break;
    case (Action::Reduce):
      bool reduced = Reduce(todo);
      if (!reduced) {
        return false;
      }
      break;
    }

    todo = table.Get(cur_state, word[word_idx]);
  }
  return true;
}
