// NOLINT (copyright/legal)

#include "aethermark/rules_block/rules_block.hpp"

#include <gtest/gtest.h>

#include <any>
#include <deque>

#include "aethermark/aethermark.hpp"
#include "aethermark/rules_block/state_block.hpp"
#include "aethermark/token.hpp"

namespace am = aethermark;

namespace {

TEST(Blockquote, RejectsWhenIndentedTooMuch) {
  am::Aethermark md;
  std::deque<am::Token> tokens_out;
  am::StateBlock state("    > hello", md, {}, tokens_out);

  state.s_count[0] = 4;
  state.blk_indent = 0;

  EXPECT_FALSE(am::BlockRules::RuleBlockquote(state, 0, 1, false));
}

TEST(Blockquote, RejectsWhenNoMarker) {
  am::Aethermark md;
  std::deque<am::Token> tokens_out;
  am::StateBlock state("hello", md, {}, tokens_out);

  state.s_count[0] = 0;
  state.b_marks[0] = 0;
  state.t_shift[0] = 0;
  state.e_marks[0] = 5;

  EXPECT_FALSE(am::BlockRules::RuleBlockquote(state, 0, 1, false));
}

TEST(Blockquote, SilentModeReturnsTrue) {
  am::Aethermark md;
  std::deque<am::Token> tokens_out;
  am::StateBlock state("> test", md, {}, tokens_out);

  state.b_marks[0] = 0;
  state.t_shift[0] = 0;
  state.e_marks[0] = 6;

  EXPECT_TRUE(am::BlockRules::RuleBlockquote(state, 0, 1, true));
};

TEST(Blockquote, BasicBlockquoteParsesCorrectly) {
  am::Aethermark md("commonmark");
  std::deque<am::Token> tokens_out;
  am::StateBlock state("> hello\nnext", md, {}, tokens_out);

  // Init line positions
  state.b_marks = {0, 7};
  state.t_shift = {0, 0};
  state.e_marks = {7, 11};
  state.s_count = {0, 0};
  state.bs_count = {0, 0};
  state.line_max = 2;

  bool ok = am::BlockRules::RuleBlockquote(state, 0, 3, false);
  EXPECT_TRUE(ok);

  ASSERT_GE(state.tokens.size(), 2);
  EXPECT_EQ(state.tokens[0].type, "blockquote_open");
  EXPECT_EQ(state.tokens[1].type, "paragraph_open");
  EXPECT_EQ(state.tokens[2].type, "inline");
  EXPECT_EQ(state.tokens[3].type, "paragraph_close");
  EXPECT_EQ(state.tokens[4].type, "blockquote_close");

  EXPECT_EQ(state.tokens[0].markup, ">");

  EXPECT_TRUE(state.tokens[0].map.has_value());
}

TEST(Blockquote, BlankLineStopsBlockquote) {
  am::Aethermark md("commonmark");
  std::deque<am::Token> tokens_out;
  am::StateBlock state("> hi\n\nrest", md, {}, tokens_out);

  state.b_marks = {0, 4, 5};
  state.t_shift = {0, 0, 0};
  state.e_marks = {4, 4, 9};
  state.s_count = {0, 0, 0};
  state.bs_count = {0, 0, 0};

  bool ok = am::BlockRules::RuleBlockquote(state, 0, 3, false);
  EXPECT_TRUE(ok);

  EXPECT_EQ(state.tokens[0].type, "blockquote_open");
  EXPECT_EQ(state.tokens.back().type, "blockquote_close");
}

TEST(Blockquote, TerminatorRuleTriggersBreak) {
  am::Aethermark md("commonmark");
  std::deque<am::Token> tokens_out;

  // Inject fake terminator for "blockquote"
  md.block_parser.ruler.Push(
      "psudo_terminator",
      [](const am::StateBlock& st, int ln, int end, bool silent) {
        return true;  // force terminate
      },
      am::RuleOptions({"blockquote"}));

  am::StateBlock state("> hi\n> yo", md, {}, tokens_out);

  state.b_marks = {0, 4};
  state.t_shift = {0, 0};
  state.e_marks = {4, 8};
  state.s_count = {0, 0};
  state.line_max = 2;

  bool ok = am::BlockRules::RuleBlockquote(state, 0, 2, false);
  EXPECT_TRUE(ok);
}

TEST(Blockquote, HandlesTabAfterMarker) {
  am::Aethermark md("commonmark");
  std::deque<am::Token> tokens_out;
  am::StateBlock state(">\ttext", md, {}, tokens_out);

  state.b_marks = {0};
  state.t_shift = {0};
  state.e_marks = {6};
  state.s_count = {0};
  state.bs_count = {0};

  bool ok = am::BlockRules::RuleBlockquote(state, 0, 1, false);
  EXPECT_TRUE(ok);
}

TEST(Blockquote, ParagraphContinuation) {
  am::Aethermark md("commonmark");
  std::deque<am::Token> tokens_out;

  am::StateBlock state("> a\n  b", md, {}, tokens_out);

  state.b_marks = {0, 4};
  state.t_shift = {0, 2};
  state.e_marks = {4, 5};
  state.s_count = {0, 2};
  state.bs_count = {0, 0};

  bool ok = am::BlockRules::RuleBlockquote(state, 0, 2, false);
  EXPECT_TRUE(ok);
}

TEST(Blockquote, RestoresOffsetsAfterParsing) {
  am::Aethermark md("commonmark");
  std::deque<am::Token> tokens_out;

  am::StateBlock state("> x", md, {}, tokens_out);

  state.b_marks = {0};
  state.t_shift = {0};
  state.e_marks = {3};
  state.s_count = {0};
  state.bs_count = {0};

  bool ok = am::BlockRules::RuleBlockquote(state, 0, 1, false);
  EXPECT_TRUE(ok);

  EXPECT_EQ(state.b_marks[0], 0);
  EXPECT_EQ(state.t_shift[0], 0);
}

}  // namespace
