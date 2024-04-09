#pragma once
#include "grammar.hpp"

struct State {
  Rule rule;
  size_t rule_pos = 0;
  Token predicted_token;

  bool IsDone() const;

  Token Get() const;

  string ToString() const;
};

template <> struct std::hash<State> {
  size_t operator()(const State &state) const {
    ;
    return hash<string>()(state.ToString());
  }
};

bool operator==(const State &, const State &);

ostream &operator<<(ostream &, const State &);
