#include "table.hpp"

bool operator!=(const Action &lhs, const Action &rhs) {
  return (lhs.type != rhs.type || lhs.to != rhs.to);
}

bool LRTable::BuildTable() {
  LRAutomata aut(grammar);
  aut.BuildAutomata();
  size_t sz = aut.nodes.size();
  table.resize(sz);

  for (size_t idx = 0; idx < sz; ++idx) {
    for (const Token &non_term : grammar.non_terminals) {
      table[idx][non_term] =
          Action{.type = Action::Goto, .to = aut.nodes[idx].children[non_term]};
    }
    for (const Token &term : grammar.terminals) {
      table[idx][term] =
          Action{.type = Action::Shift, .to = aut.nodes[idx].children[term]};
    }
  }

  for (size_t idx = 0; idx < sz; ++idx) {
    for (const State &state : aut.nodes.at(idx).stateset) {
      if (state.IsDone()) {
        if (table[idx].find(state.predicted_token) != table[idx].end()) {
          if (table[idx][state.predicted_token] &&
              table[idx][state.predicted_token] !=
                  Action{.type = Action::Reduce,
                         .to = grammar.rule_idx.at(state.rule)}) {
            return false;
          }
        }
        table[idx][state.predicted_token] = Action{
            .type = Action::Reduce, .to = grammar.rule_idx.at(state.rule)};
      }
    }
  }
  return true;
}

Action LRTable::Get(size_t idx, const Token &token) {
  return table.at(idx)[token];
}