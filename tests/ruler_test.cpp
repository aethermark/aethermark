// NOLINT (copyright/legal)

#include "aethermark/ruler.hpp"

#include <gtest/gtest.h>

#include <functional>
#include <string>
#include <vector>

using Fn = std::function<void(int&)>;
namespace am = aethermark;

// Helpers
static void add1(int& x) { x += 1; }
static void add2(int& x) { x += 2; }
static void mul2(int& x) { x *= 2; }

namespace {

// ---------- Insertion ----------

TEST(Ruler, PushAddsRules) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("b", add2);

  auto rules = r.GetRules("");

  ASSERT_EQ(rules.size(), 2);
  int x = 0;
  rules[0].second(x);
  rules[1].second(x);
  EXPECT_EQ(x, 3);
}

TEST(Ruler, BeforeInsertsCorrectly) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("c", add2);

  r.Before("c", "b", mul2);

  auto rules = r.GetRules("");

  ASSERT_EQ(rules.size(), 3);

  int x = 1;
  for (auto& fn : rules) fn.second(x);
  EXPECT_EQ(x, ((1 + 1) * 2) + 2);
}

TEST(Ruler, AfterInsertsCorrectly) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("c", add2);

  r.After("a", "b", mul2);

  auto rules = r.GetRules("");

  ASSERT_EQ(rules.size(), 3);

  int x = 1;
  for (auto& fn : rules) fn.second(x);
  EXPECT_EQ(x, 6);
}

TEST(Ruler, AtReplacesRule) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("b", add2);

  r.At("a", mul2);

  auto rules = r.GetRules("");

  int x = 2;
  rules[0].second(x);
  EXPECT_EQ(x, 4);
}

// ---------- EnableDisable ----------

TEST(Ruler, EnableDisableWork) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("b", add2);
  r.Push("c", mul2);

  r.Disable("b");

  auto rules = r.GetRules("");
  ASSERT_EQ(rules.size(), 2);

  int x = 1;
  for (auto& fn : rules) fn.second(x);
  EXPECT_EQ(x, (1 + 1) * 2);
}

TEST(Ruler, EnableOnlyWorks) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("b", add2);
  r.Push("c", mul2);

  r.EnableOnly({"b", "c"});

  auto rules = r.GetRules("");
  ASSERT_EQ(rules.size(), 2);

  int x = 1;
  for (auto& fn : rules) fn.second(x);
  EXPECT_EQ(x, (1 + 2) * 2);
}

TEST(Ruler, IgnoredInvalidEnable) {
  am::Ruler<Fn> r;
  r.Push("a", add1);

  EXPECT_NO_THROW(r.Enable("zzz", true));
}

TEST(Ruler, IgnoredInvalidDisable) {
  am::Ruler<Fn> r;
  r.Push("a", add1);

  EXPECT_NO_THROW(r.Disable("zzz", true));
}

// ---------- AltChain ----------

TEST(Ruler, AltChainsWork) {
  am::Ruler<Fn> r;

  r.Push("a", add1, {{"alt1"}});
  r.Push("b", add2, {{"alt2"}});
  r.Push("c", mul2, {{"alt1"}});

  auto alt1 = r.GetRules("alt1");
  auto alt2 = r.GetRules("alt2");
  auto def = r.GetRules("");

  ASSERT_EQ(alt1.size(), 2);
  ASSERT_EQ(alt2.size(), 1);
  ASSERT_EQ(def.size(), 3);

  int x = 1;
  for (auto& fn : alt1) fn.second(x);
  EXPECT_EQ(x, (1 + 1) * 2);
}

// ---------- Lookup ----------

TEST(Ruler, FindRuleWorks) {
  am::Ruler<Fn> r;

  r.Push("a", add1);
  r.Push("b", add2);

  auto* rule = r.FindRule("b");
  ASSERT_NE(rule, nullptr);
  EXPECT_EQ(rule->name, "b");

  EXPECT_EQ(r.FindRule("z"), nullptr);
}

TEST(Ruler, ThrowsOnMissingAt) {
  am::Ruler<Fn> r;
  r.Push("a", add1);

  EXPECT_THROW(r.At("nope", add2), std::runtime_error);
}

TEST(Ruler, ThrowsOnMissingBefore) {
  am::Ruler<Fn> r;
  r.Push("a", add1);

  EXPECT_THROW(r.Before("nope", "b", add2), std::runtime_error);
}

TEST(Ruler, ThrowsOnMissingAfter) {
  am::Ruler<Fn> r;
  r.Push("a", add1);

  EXPECT_THROW(r.After("nope", "b", add2), std::runtime_error);
}

}  // namespace
