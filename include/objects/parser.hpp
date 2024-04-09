#pragma once
#include "table.hpp"

struct LRParser {
  LRGrammar grammar;
  LRTable table;

  stack<int> int_stack;
  stack<Token> token_stack;

  int cur_state;
  int word_idx;

  LRParser(const LRGrammar &grammar) : grammar(grammar), table(grammar) {}

  void BuildTable();

  void Shift(const Action &todo, const Token &token);

  bool Reduce(const Action &todo);

  bool Predict(Word word);
};
