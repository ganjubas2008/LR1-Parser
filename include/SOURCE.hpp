#pragma once
#include <bits/stdc++.h>
using namespace std;
#define Token string
#define Word vector<Token>

const string kImplies = "::=";
const string kDelimiter = "|";
const string kLeftBound = "<";
const string kRightBound = ">";
const Token kEmptyToken = "";
const Word kEmptyWord = {kEmptyToken};

template <> struct std::hash<Word> {
  size_t operator()(const Word& word) const {
    string str = "";
    for (const auto& token: word) {
        str += kLeftBound + token + kRightBound;
    }
    return hash<string>()(str);
  }
};

template<typename Obj>
bool InsertSet(set<Obj>& lhs, const set<Obj>& rhs) {
    bool changes = false;
    for (const Obj& obj: rhs) {
        changes |= lhs.insert(obj).second;
    }
    return changes;
}