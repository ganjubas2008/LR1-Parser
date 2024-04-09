#include "test_parser.hpp"
#include "reader.hpp"

bool TestDir(const string& dirname) {
    LRGrammar grammar = GrammarReader().Read(dirname + "/grammar.txt");
    vector<TestWord> testwords = TestWordsReader().Read(dirname + "/test_words.txt");
    LRParser parser(grammar);
    parser.BuildTable();

    bool okay = true;
    for (const auto& testword : testwords) {
        okay &= (parser.Predict(testword.word) == testword.ans);
    }
    return true;
}

TEST_F(LRParserTest, NonLRGrammar) {
  Rule r0("S", {"A"});
  Rule r1("A", {"B", "a"});
  Rule r2("B", {"b"});
  Rule r3("A", {"B", "A", "B"});
  Rule r4("A", {"a", "A"});
  LRGrammar grammar({r0, r1, r2, r3, r4});
  LRParser parser(grammar);
  EXPECT_THROW(parser.BuildTable(), invalid_argument);
}

TEST_F(LRParserTest, Simple_1) {
    ASSERT_TRUE(TestDir("../tests/test_simple"));
}

TEST_F(LRParserTest, Simple_2) {
    ASSERT_TRUE(TestDir("../tests/test_lr"));
}

TEST_F(LRParserTest, Simple_3) {
    ASSERT_TRUE(TestDir("../tests/test_bs"));
}