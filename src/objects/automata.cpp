#include "automata.hpp"

#define StateSet unordered_set<State>

int LRAutomata::_GetStateSetIdx(const StateSet &stateset) {
  return find_if(nodes.begin(), nodes.end(),
                 [stateset](const auto &node) {
                   return node.stateset == stateset;
                 }) -
         nodes.begin();
}

void LRAutomata::BuildAutomata() {
  grammar.PrecalcFirstToken();
  StateSet states0{State{
      .rule = kRuleBegin,
      .rule_pos = 0,
      .predicted_token = "$",
  }};
  _CloseStateSet(states0);
  nodes.push_back(Node{// Добавляем пустое состояние
                       .stateset = {},
                       .idx = 0,
                       .children = {},
                       .complete = false});
  nodes.push_back(Node{// Добавляем изначальное состояние (Sp -> S, $)
                       .stateset = states0,
                       .idx = 1,
                       .children = {},
                       .complete = false});
  _BuildRecursive(1);
}

void LRAutomata::_BuildRecursive(int idx) {
  if (nodes[idx].complete) {
    return;
  }
  nodes[idx].complete = true;
  for (const Token &token : grammar.alphabet) {
    if (nodes[idx].children.find(token) != nodes[idx].children.end()) {
      continue;
    }
    StateSet new_stateset = _Goto(nodes[idx].stateset, token);
    if (_GetStateSetIdx(new_stateset) == nodes.size()) {
      Node newnode{.stateset = new_stateset,
                   .idx = nodes.size(),
                   .children = {},
                   .complete = false};
      nodes.push_back(newnode);
      _BuildRecursive(newnode.idx);
    }
    nodes[idx].children[token] = _GetStateSetIdx(new_stateset);
  }
}

bool LRAutomata::_CloseState(const State &state, StateSet &closure) {
  bool changes = false;
  if (grammar.IsNonTerm(state.Get())) {
    Word new_word(state.rule.rhs.begin() + state.rule_pos + 1,
                  state.rule.rhs.end());
    if (state.predicted_token != "$") {
      new_word.push_back(state.predicted_token);
    }
    Token new_rule_lhs = state.Get();
    for (const Word &new_rule_rhs : grammar.rules.at(new_rule_lhs)) {
      for (const Token &new_predicted_token : grammar.GetFirst(new_word)) {
        changes |= closure
                       .insert(State{
                           .rule = Rule{new_rule_lhs, new_rule_rhs},
                           .rule_pos = 0,
                           .predicted_token = new_predicted_token,
                       })
                       .second;
      }
    }
  }
  return changes;
}

void LRAutomata::_CloseStateSet(StateSet &closure) {
  bool changes = true;
  while (changes) {
    changes = false;
    for (const State &state : closure) {
      changes |= _CloseState(state, closure);
    }
  }
}

StateSet LRAutomata::_Goto(const StateSet &stateset, const Token &token) {
  StateSet result;
  for (const State &state : stateset) {
    if (state.Get() == token) {
      result.insert(State{
          .rule = state.rule,
          .rule_pos = state.rule_pos + 1,
          .predicted_token = state.predicted_token,
      });
    }
  }
  _CloseStateSet(result);
  return result;
}