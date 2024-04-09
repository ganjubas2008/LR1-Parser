#pragma once
#include "automata.hpp"

struct Action {
  int type = 0;
  int to;
  enum types { None = 0, Goto = 1, Shift = 2, Reduce = 3 };

  operator bool() const {
    return !(type == None || (type != Reduce && to == 0));
  }
};

bool operator!=(const Action &lhs, const Action &rhs);

ostream &operator<<(ostream &os, const Action &action);

struct LRTable {
  vector<unordered_map<Token, Action>> table;
  LRGrammar grammar;

  LRTable(LRGrammar grammar) : grammar(grammar) {}

  bool BuildTable();

  Action Get(size_t idx, const Token &token);
};