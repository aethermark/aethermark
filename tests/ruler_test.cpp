// NOLINT (copyright/legal)

#include "aethermark/ruler.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace aethermark {

// Dummy state type for rules
struct MockState {
  int value = 0;
};

// Define a function type for rule callbacks
using RuleFn = std::function<void(MockState&)>;

// Fixture for Ruler tests
class RulerTest : public ::testing::Test {
 protected:
  Ruler<RuleFn> ruler;

  void SetUp() override {
    // start each test with an empty ruler
    ruler = Ruler<RuleFn>();
  }
};

//
// ─── CONSTRUCTION AND BASIC INSERTION
// ─────────────────────────────────────────
//

TEST_F(RulerTest, StartsEmpty) {
  EXPECT_TRUE(ruler.getRules().empty());
  EXPECT_TRUE(ruler.getCache().empty());
}

TEST_F(RulerTest, PushAddsRuleAtEnd) {
  ruler.push("rule1", [](MockState&) {});
  ruler.push("rule2", [](MockState&) {});
  auto rules = ruler.getRules();
  ASSERT_EQ(rules.size(), 2);
  EXPECT_EQ(rules[0].name, "rule1");
  EXPECT_EQ(rules[1].name, "rule2");
}

// //
// // ─── BEFORE / AFTER / AT
// // ───────────────────────────────────────────────────────
// //

// TEST_F(RulerTest, BeforeInsertsBeforeNamedRule) {
//   ruler.push("a", [](MockState&) {});
//   ruler.push("b", [](MockState&) {});
//   ruler.before("b", "x", [](MockState&) {});
//   auto rules = ruler.getRules();
//   ASSERT_EQ(rules[1].name, "x");
// }

// TEST_F(RulerTest, AfterInsertsAfterNamedRule) {
//   ruler.push("a", [](MockState&) {});
//   ruler.push("b", [](MockState&) {});
//   ruler.after("a", "x", [](MockState&) {});
//   auto rules = ruler.getRules();
//   ASSERT_EQ(rules[1].name, "x");
// }

// TEST_F(RulerTest, AtReplacesRuleFunction) {
//   bool called = false;
//   ruler.push("a", [&](MockState&) {});
//   ruler.at("a", [&](MockState&) { called = true; });
//   auto rules = ruler.getRules();
//   MockState s;
//   rules[0].fn(s);
//   EXPECT_TRUE(called);
// }

// TEST_F(RulerTest, BeforeThrowsWhenNotFound) {
//   EXPECT_THROW(ruler.before("missing", "x", [](MockState&) {}),
//                std::runtime_error);
// }

// TEST_F(RulerTest, AfterThrowsWhenNotFound) {
//   EXPECT_THROW(ruler.after("missing", "x", [](MockState&) {}),
//                std::runtime_error);
// }

// TEST_F(RulerTest, AtThrowsWhenNotFound) {
//   EXPECT_THROW(ruler.at("missing", [](MockState&) {}), std::runtime_error);
// }

// //
// // ─── ENABLE / DISABLE
// // ──────────────────────────────────────────────────────────
// //

// TEST_F(RulerTest, EnableAndDisableSingleRule) {
//   ruler.push("r1", [](MockState&) {});
//   ruler.push("r2", [](MockState&) {});
//   ruler.disable("r1");
//   auto rules = ruler.getRules();
//   EXPECT_FALSE(rules[0].enabled);

//   ruler.enable("r1");
//   rules = ruler.getRules();
//   EXPECT_TRUE(rules[0].enabled);
// }

// TEST_F(RulerTest, EnableThrowsForMissingUnlessIgnored) {
//   EXPECT_THROW(ruler.enable("missing"), std::runtime_error);
//   EXPECT_NO_THROW(ruler.enable("missing", true));
// }

// TEST_F(RulerTest, DisableThrowsForMissingUnlessIgnored) {
//   EXPECT_THROW(ruler.disable("missing"), std::runtime_error);
//   EXPECT_NO_THROW(ruler.disable("missing", true));
// }

// TEST_F(RulerTest, EnableOnlyKeepsWhitelist) {
//   ruler.push("a", [](MockState&) {});
//   ruler.push("b", [](MockState&) {});
//   ruler.enableOnly("a");
//   auto rules = ruler.getRules();
//   EXPECT_TRUE(rules[0].enabled);
//   EXPECT_FALSE(rules[1].enabled);
// }

// //
// // ─── CACHE / COMPILATION
// // ───────────────────────────────────────────────────────
// //

// TEST_F(RulerTest, CacheInvalidatedAfterModification) {
//   ruler.push("a", [](MockState&) {});
//   auto cache1 = ruler.getCache();
//   ruler.push("b", [](MockState&) {});
//   auto cache2 = ruler.getCache();
//   EXPECT_NE(cache1.size(), cache2.size());
// }

// TEST_F(RulerTest, CompileSkipsDisabledRules) {
//   ruler.push("a", [](MockState&) {});
//   ruler.push("b", [](MockState&) {});
//   ruler.disable("a");
//   auto active = ruler.getRules("");
//   ASSERT_EQ(active.size(), 1);
//   EXPECT_EQ(active[0].name, "b");
// }

// //
// // ─── ALT CHAIN HANDLING
// // ────────────────────────────────────────────────────────
// //

// TEST_F(RulerTest, AltChainsAreRespected) {
//   ruler.push("r1", [](MockState&) {}, {"alt1"});
//   ruler.push("r2", [](MockState&) {}, {"alt2"});
//   auto alt1 = ruler.getRules("alt1");
//   auto alt2 = ruler.getRules("alt2");
//   EXPECT_EQ(alt1.size(), 1);
//   EXPECT_EQ(alt1[0].name, "r1");
//   EXPECT_EQ(alt2.size(), 1);
//   EXPECT_EQ(alt2[0].name, "r2");
// }

// TEST_F(RulerTest, DisabledRulesAreOmittedFromAltChains) {
//   ruler.push("r1", [](MockState&) {}, {"alt"});
//   ruler.disable("r1");
//   auto alt = ruler.getRules("alt");
//   EXPECT_TRUE(alt.empty());
// }

// //
// // ─── MULTIPLE ENABLED / DISABLED NAMES
// // ─────────────────────────────────────────
// //

// TEST_F(RulerTest, EnableMultipleRulesByList) {
//   ruler.push("r1", [](MockState&) {});
//   ruler.push("r2", [](MockState&) {});
//   ruler.disable({"r1", "r2"});
//   ruler.enable({"r1", "r2"});
//   for (auto& r : ruler.getRules()) EXPECT_TRUE(r.enabled);
// }

// TEST_F(RulerTest, DisableMultipleRulesByList) {
//   ruler.push("r1", [](MockState&) {});
//   ruler.push("r2", [](MockState&) {});
//   ruler.disable({"r1", "r2"});
//   for (auto& r : ruler.getRules()) EXPECT_FALSE(r.enabled);
// }

// //
// // ─── FUNCTIONAL INVOCATION
// // ─────────────────────────────────────────────────────
// //

// TEST_F(RulerTest, FunctionsExecuteInInsertionOrder) {
//   std::vector<std::string> callOrder;
//   ruler.push("a", [&](MockState&) { callOrder.push_back("a"); });
//   ruler.push("b", [&](MockState&) { callOrder.push_back("b"); });

//   MockState s;
//   for (auto& r : ruler.getRules("")) r.fn(s);

//   ASSERT_EQ(callOrder.size(), 2);
//   EXPECT_EQ(callOrder[0], "a");
//   EXPECT_EQ(callOrder[1], "b");
// }

// TEST_F(RulerTest, ReplacedRuleExecutesNewFunction) {
//   bool called = false;
//   ruler.push("r1", [&](MockState&) {});
//   ruler.at("r1", [&](MockState&) { called = true; });
//   MockState s;
//   ruler.getRules("")[0].fn(s);
//   EXPECT_TRUE(called);
// }

// //
// // ─── ERROR HANDLING
// // ────────────────────────────────────────────────────────────
// //

// TEST_F(RulerTest, GetRulesReturnsEmptyForUnknownChain) {
//   ruler.push("r1", [](MockState&) {});
//   auto chain = ruler.getRules("does_not_exist");
//   EXPECT_TRUE(chain.empty());
// }

// TEST_F(RulerTest, ThrowsOnAtInvalidRule) {
//   EXPECT_THROW(ruler.at("invalid", [](MockState&) {}), std::runtime_error);
// }

}  // namespace aethermark
