#include "test_table.hpp"

TEST_F(LRTableTest, IsGrammarLR1_1) {
  Rule r1{"S", {"C", "C"}};
  Rule r2{"C", {"c", "C"}};
  Rule r3{"C", {"d"}};
  LRGrammar grammar({r1, r2, r3});
  LRTable table(grammar);
  bool is_grammar_lr1 = table.BuildTable();
  ASSERT_TRUE(is_grammar_lr1);
}

TEST_F(LRTableTest, IsGrammarLR1_2) {
  Rule r1{"S", {"S", "a", "S", "b"}};
  Rule r2{"S", {""}};
  LRGrammar grammar({r1, r2});
  LRTable table(grammar);
  bool is_grammar_lr1 = table.BuildTable();
  ASSERT_TRUE(is_grammar_lr1);
}

TEST_F(LRTableTest, IsGrammarLR1_3) {
  Rule r1{"S", {""}};
  Rule r2{"S", {"a", "S", "a"}};
  Rule r3{"S", {"b", "S", "b"}};
  LRGrammar grammar({r1, r2, r3});
  LRTable table(grammar);
  bool is_grammar_lr1 = table.BuildTable();
  ASSERT_FALSE(is_grammar_lr1);
}
