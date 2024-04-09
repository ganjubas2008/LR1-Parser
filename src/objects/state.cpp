#include "state.hpp"

bool State::IsDone() const {
  return rule.rhs == kEmptyWord || rule_pos == rule.rhs.size();
}

Token State::Get() const { return (IsDone() ? "$" : rule.rhs[rule_pos]); }

string State::ToString() const {
  string ans = "(" + rule.ToString() + " " + to_string(rule_pos) + " " +
               kDelimiter + " {";
  ans += " ";
  ans += kLeftBound + predicted_token + kRightBound;
  ans += "}";
  return ans;
}

bool operator==(const State &lhs, const State &rhs) {
  return lhs.ToString() == rhs.ToString();
}

ostream &operator<<(ostream &os, const State &state) {
  os << state.ToString();
  return os;
}
