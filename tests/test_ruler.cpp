// NOLINT (copyright/legal)

#include <gtest/gtest.h>

#include <functional>
#include <string>
#include <vector>

#include "aethermark/ruler.hpp"

using Fn = std::function<void(int&)>;
namespace am = aethermark;

// Helpers
static void add1(int& x) { x += 1; }
static void add2(int& x) { x += 2; }
static void mul2(int& x) { x *= 2; }

namespace {

// ---------- Insertion ----------

TEST(Ruler__Insertion, PushAddsRules) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("b", add2);

  auto rules = r.getRules("");

  ASSERT_EQ(rules.size(), 2);
  int x = 0;
  rules[0](x);
  rules[1](x);
  EXPECT_EQ(x, 3);
}

TEST(Ruler__Insertion, BeforeInsertsCorrectly) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("c", add2);

  r.before("c", "b", mul2);

  auto rules = r.getRules("");

  ASSERT_EQ(rules.size(), 3);

  int x = 1;
  for (auto& fn : rules) fn(x);
  EXPECT_EQ(x, ((1 + 1) * 2) + 2);
}

TEST(Ruler__Insertion, AfterInsertsCorrectly) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("c", add2);

  r.after("a", "b", mul2);

  auto rules = r.getRules("");

  ASSERT_EQ(rules.size(), 3);

  int x = 1;
  for (auto& fn : rules) fn(x);
  EXPECT_EQ(x, 6);
}

TEST(Ruler__Insertion, AtReplacesRule) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("b", add2);

  r.at("a", mul2);

  auto rules = r.getRules("");

  int x = 2;
  rules[0](x);
  EXPECT_EQ(x, 4);
}

// ---------- EnableDisable ----------

TEST(Ruler__EnableDisable, EnableDisableWork) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("b", add2);
  r.push("c", mul2);

  r.disable("b");

  auto rules = r.getRules("");
  ASSERT_EQ(rules.size(), 2);

  int x = 1;
  for (auto& fn : rules) fn(x);
  EXPECT_EQ(x, (1 + 1) * 2);
}

TEST(Ruler__EnableDisable, EnableOnlyWorks) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("b", add2);
  r.push("c", mul2);

  r.enableOnly({"b", "c"});

  auto rules = r.getRules("");
  ASSERT_EQ(rules.size(), 2);

  int x = 1;
  for (auto& fn : rules) fn(x);
  EXPECT_EQ(x, (1 + 2) * 2);
}

TEST(Ruler__EnableDisable, IgnoredInvalidEnable) {
  am::Ruler<Fn> r;
  r.push("a", add1);

  EXPECT_NO_THROW(r.enable("zzz", true));
}

TEST(Ruler__EnableDisable, IgnoredInvalidDisable) {
  am::Ruler<Fn> r;
  r.push("a", add1);

  EXPECT_NO_THROW(r.disable("zzz", true));
}

// ---------- AltChain ----------

TEST(Ruler__AltChain, AltChainsWork) {
  am::Ruler<Fn> r;

  r.push("a", add1, {{"alt1"}});
  r.push("b", add2, {{"alt2"}});
  r.push("c", mul2, {{"alt1"}});

  auto alt1 = r.getRules("alt1");
  auto alt2 = r.getRules("alt2");
  auto def = r.getRules("");

  ASSERT_EQ(alt1.size(), 2);
  ASSERT_EQ(alt2.size(), 1);
  ASSERT_EQ(def.size(), 3);

  int x = 1;
  for (auto& fn : alt1) fn(x);
  EXPECT_EQ(x, (1 + 1) * 2);
}

// ---------- Lookup ----------

TEST(Ruler__Lookup, FindRuleWorks) {
  am::Ruler<Fn> r;

  r.push("a", add1);
  r.push("b", add2);

  auto* rule = r.findRule("b");
  ASSERT_NE(rule, nullptr);
  EXPECT_EQ(rule->name, "b");

  EXPECT_EQ(r.findRule("z"), nullptr);
}

TEST(Ruler__Lookup, ThrowsOnMissingAt) {
  am::Ruler<Fn> r;
  r.push("a", add1);

  EXPECT_THROW(r.at("nope", add2), std::runtime_error);
}

TEST(Ruler__Lookup, ThrowsOnMissingBefore) {
  am::Ruler<Fn> r;
  r.push("a", add1);

  EXPECT_THROW(r.before("nope", "b", add2), std::runtime_error);
}

TEST(Ruler__Lookup, ThrowsOnMissingAfter) {
  am::Ruler<Fn> r;
  r.push("a", add1);

  EXPECT_THROW(r.after("nope", "b", add2), std::runtime_error);
}

}  // namespace
