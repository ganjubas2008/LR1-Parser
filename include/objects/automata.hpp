#pragma once
#include "grammar.hpp"
#include "state.hpp"

#define StateSet unordered_set<State>

class LRAutomata {
public:
  /*
  Автомат состоит из нодов, хранящих StateSet
  Каждый StateSet символизирует класс эквивалентности состояний Эрли
  */
  struct Node {
    StateSet stateset;
    size_t idx; // номер состояния
    unordered_map<Token, int>
        children; // номер состояния, в которое попадём, перейдя по токену
    bool complete; // обошли ли всех детей данной вершины
  };

  vector<Node> nodes;
  LRGrammar grammar;

  LRAutomata(const LRGrammar &grammar) : grammar(grammar){};

  void BuildAutomata();

private:
  int _GetStateSetIdx(
      const StateSet &); // Возращает номер вершины, хранящей такой stateset

  void _BuildRecursive(int idx);

  bool _CloseState(const State &state, StateSet &closure);

  void _CloseStateSet(StateSet &closure);

  StateSet _Goto(const StateSet &stateset, const Token &token);
};
