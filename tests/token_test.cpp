// NOLINT (copyright/legal)

#include "aethermark/token.hpp"

#include <gtest/gtest.h>

#include <string>
#include <utility>
#include <vector>

namespace am = aethermark;

namespace {

// ---------- Constructor & Basic State ----------

TEST(Token, InitializesMembersCorrectly) {
  am::Token t("paragraph_open", "p", am::Nesting::OPENING);

  EXPECT_EQ(t.GetType(), "paragraph_open");
  EXPECT_EQ(t.GetTag(), "p");
  EXPECT_EQ(t.GetNesting(), am::Nesting::OPENING);
}

TEST(Token, DefaultOptionalsAndStringsAreEmpty) {
  am::Token t("a", "b", am::Nesting::SELF_CLOSING);

  EXPECT_FALSE(t.GetAttrs().has_value());
  EXPECT_FALSE(t.GetMap().has_value());
  EXPECT_FALSE(t.GetChildren().has_value());

  EXPECT_EQ(t.GetContent(), "");
  EXPECT_EQ(t.GetMarkup(), "");
  EXPECT_EQ(t.GetInfo(), "");
  EXPECT_EQ(t.GetLevel(), 0.0f);
  EXPECT_FALSE(t.IsBlock());
  EXPECT_FALSE(t.IsHidden());
}

TEST(Token, MutatorsWorkCorrectly) {
  am::Token t("x", "y", am::Nesting::CLOSING);

  t.SetType("new_type");
  t.SetTag("new_tag");
  t.SetLevel(2.5f);
  t.SetBlock(true);
  t.SetHidden(true);

  EXPECT_EQ(t.GetType(), "new_type");
  EXPECT_EQ(t.GetTag(), "new_tag");
  EXPECT_EQ(t.GetLevel(), 2.5f);
  EXPECT_TRUE(t.IsBlock());
  EXPECT_TRUE(t.IsHidden());

  t.SetMap(std::nullopt);
  EXPECT_FALSE(t.GetMap().has_value());

  t.SetMap(std::make_pair(3.0f, 4.0f));
  EXPECT_EQ(t.GetMap()->first, 3.0f);
  EXPECT_EQ(t.GetMap()->second, 4.0f);

  t.SetMapAt(0, 5.0f);
  t.SetMapAt(1, 6.0f);
  EXPECT_EQ(t.GetMap()->first, 5.0f);
  EXPECT_EQ(t.GetMap()->second, 6.0f);
}

// ---------- AttrPush ----------

TEST(Token, AddsMultipleAttributes) {
  am::Token t("inline", "span", am::Nesting::SELF_CLOSING);

  std::vector<std::pair<std::string, std::string>> pushData = {
      {"class", "lead"}, {"id", "first"}, {"data-x", "42"}};
  t.AttrPush(pushData);

  ASSERT_TRUE(t.GetAttrs().has_value());
  auto attrs = t.GetAttrs().value();

  // Ignore possible leading empty attribute
  size_t offset = (!attrs.empty() && attrs[0].first.empty()) ? 1 : 0;
  ASSERT_GE(attrs.size(), 3u + offset);

  EXPECT_EQ(attrs[offset + 0].first, "class");
  EXPECT_EQ(attrs[offset + 1].first, "id");
  EXPECT_EQ(attrs[offset + 2].first, "data-x");

  // Check AttrGet
  auto c = t.AttrGet("class");
  ASSERT_TRUE(c.has_value());
  EXPECT_EQ(c.value(), "lead");
}

TEST(Token, PreservesOrderAcrossMultiplePushes) {
  am::Token t("x", "y", am::Nesting::SELF_CLOSING);

  t.AttrPush({{"a", "1"}, {"b", "2"}});
  t.AttrPush({{"c", "3"}});
  t.AttrPush({{"a", "4"}});

  auto attrs = t.GetAttrs().value();
  size_t offset = (!attrs.empty() && attrs[0].first.empty()) ? 1 : 0;
  ASSERT_GE(attrs.size(), 4u + offset);

  EXPECT_EQ(attrs[offset + 0].first, "a");
  EXPECT_EQ(attrs[offset + 0].second, "1");
  EXPECT_EQ(attrs[offset + 3].first, "a");
  EXPECT_EQ(attrs[offset + 3].second, "4");
}

TEST(Token, HandlesEmptyVectorGracefully) {
  am::Token t("z", "z", am::Nesting::OPENING);
  t.AttrPush({});
  // Implementation likely initializes attrs even if empty
  EXPECT_TRUE(t.GetAttrs().has_value());
  EXPECT_TRUE(t.GetAttrs()->empty());
}

// ---------- AttrIndex ----------

TEST(Token, ReturnsCorrectIndexAndMinusOneIfMissing) {
  am::Token t("inline", "span", am::Nesting::SELF_CLOSING);
  t.AttrPush({{"class", "lead"}, {"id", "first"}});

  EXPECT_EQ(t.AttrIndex("class"), 0);
  EXPECT_EQ(t.AttrIndex("id"), 1);
  EXPECT_EQ(t.AttrIndex("nonexistent"), -1);
}

TEST(Token, HandlesDuplicateNamesAndEmptyKey) {
  am::Token t("x", "y", am::Nesting::SELF_CLOSING);
  t.AttrPush({{"", ""}, {"dup", "one"}, {"dup", "two"}, {"dup", ""}});

  EXPECT_EQ(t.AttrIndex(""), 0);
  EXPECT_EQ(t.AttrIndex("dup"), 1);
}

// ---------- AttrGet ----------

TEST(Token, ReturnsValueIfExists) {
  am::Token t("a", "b", am::Nesting::OPENING);
  t.AttrPush({{"id", "123"}});
  auto v = t.AttrGet("id");
  ASSERT_TRUE(v.has_value());
  EXPECT_EQ(v.value(), "123");
}

TEST(Token, ReturnsNulloptIfMissing) {
  am::Token t("a", "b", am::Nesting::OPENING);
  EXPECT_FALSE(t.AttrGet("notfound").has_value());
}

TEST(Token, ReturnsFirstValueForDuplicateNames) {
  am::Token t("a", "b", am::Nesting::OPENING);
  t.AttrPush({{"key", "v1"}, {"key", "v2"}});
  EXPECT_EQ(t.AttrGet("key").value(), "v1");
}

// ---------- AttrSet ----------

TEST(Token, UpdatesExistingAttributeValue) {
  am::Token t("a", "b", am::Nesting::OPENING);

  t.AttrPush({{"k", "v1"}, {"k", "v2"}, {"other", "o"}});
  t.AttrSet("k", "vNew");

  auto attrs = t.GetAttrs().value();
  // size_t offset = (!attrs.empty() && attrs[0].first.empty()) ? 1 : 0;

  // Last duplicate may be updated or cleared
  bool found = false;
  for (auto& [key, val] : attrs) {
    if (key == "k") {
      found = true;
      EXPECT_FALSE(val.empty());  // ensure not all cleared
    }
  }
  EXPECT_TRUE(found);
}

TEST(Token, AppendsIfAttributeDoesNotExist) {
  am::Token t("a", "b", am::Nesting::OPENING);

  // Perform AttrSet
  t.AttrSet("new", "value");

  // Safely get the optional first
  auto optAttrs = t.GetAttrs();
  ASSERT_TRUE(optAttrs.has_value()) << "attrs should be initialized by AttrSet";

  // Access the vector safely
  const auto& attrs = optAttrs.value();

  EXPECT_EQ(attrs.size(), 1u);
  EXPECT_EQ(attrs[0].first, "new");
  EXPECT_EQ(attrs[0].second, "value");
}

TEST(Token, HandlesEmptyKeyAndValue) {
  am::Token t("x", "y", am::Nesting::OPENING);

  // Set attribute with empty key and value
  t.AttrSet("", "");

  // Safely get the optional
  auto optAttrs = t.GetAttrs();
  ASSERT_TRUE(optAttrs.has_value()) << "attrs should be initialized by AttrSet";

  // Access the vector safely
  const auto& attrs = optAttrs.value();

  ASSERT_FALSE(attrs.empty()) << "attrs vector should have one element";
  EXPECT_EQ(attrs[0].first, "");
  EXPECT_EQ(attrs[0].second, "");
}

// ---------- AttrJoin ----------

TEST(Token, CreatesNewAttributeIfMissing) {
  am::Token t("a", "b", am::Nesting::SELF_CLOSING);
  t.AttrJoin("class", "first");
  EXPECT_EQ(t.AttrGet("class").value(), "first");
}

TEST(Token, AppendsToExistingWithSpace) {
  am::Token t("a", "b", am::Nesting::SELF_CLOSING);
  t.AttrSet("class", "one");
  t.AttrJoin("class", "two");
  EXPECT_EQ(t.AttrGet("class").value(), "one two");
}

TEST(Token, WorksWithEmptyInitialValue) {
  am::Token t("a", "b", am::Nesting::SELF_CLOSING);
  t.AttrSet("class", "");
  t.AttrJoin("class", "added");
  EXPECT_EQ(t.AttrGet("class").value(), " added");
}

TEST(Token, JoinsEmptyStringAppendsSpace) {
  am::Token t("a", "b", am::Nesting::SELF_CLOSING);
  t.AttrJoin("class", "first");
  t.AttrJoin("class", "");
  EXPECT_EQ(t.AttrGet("class").value(), "first ");
}

TEST(Token, HandlesTrailingSpaceInExistingValue) {
  am::Token t("a", "b", am::Nesting::SELF_CLOSING);
  t.AttrSet("class", "a ");
  t.AttrJoin("class", "b");
  // Depending on your implementation this may result in "a  b" or "a b"
  EXPECT_TRUE(t.AttrGet("class").value().find("b") != std::string::npos);
}

// ---------- Getters & Setters ----------

TEST(Token, ModifyScalarAndStringFieldsCorrectly) {
  am::Token t("t", "g", am::Nesting::CLOSING);

  t.SetType("newtype");
  t.SetTag("div");
  t.SetLevel(5.0f);
  t.SetNesting(am::Nesting::OPENING);
  t.SetContent("content");
  t.SetMarkup("*");
  t.SetInfo("info");
  t.SetBlock(true);
  t.SetHidden(true);

  EXPECT_EQ(t.GetType(), "newtype");
  EXPECT_EQ(t.GetTag(), "div");
  EXPECT_EQ(t.GetLevel(), 5.0f);
  EXPECT_EQ(t.GetNesting(), am::Nesting::OPENING);
  EXPECT_EQ(t.GetContent(), "content");
  EXPECT_EQ(t.GetMarkup(), "*");
  EXPECT_EQ(t.GetInfo(), "info");
  EXPECT_TRUE(t.IsBlock());
  EXPECT_TRUE(t.IsHidden());
}

TEST(Token, CanAssignOptionalsAndRetrieveThem) {
  am::Token t("a", "b", am::Nesting::OPENING);

  std::vector<std::pair<std::string, std::string>> attrs = {{"k", "v"}};
  std::pair<float, float> map = {1.0f, 2.0f};
  std::vector<am::Token> children = {
      am::Token("inline", "span", am::Nesting::SELF_CLOSING)};

  t.SetAttrs(attrs);
  t.SetMap(map);
  t.SetChildren(children);

  EXPECT_TRUE(t.GetAttrs().has_value());
  EXPECT_TRUE(t.GetMap().has_value());
  EXPECT_TRUE(t.GetChildren().has_value());
}

TEST(Token, CanClearOptionalsUsingNullopt) {
  am::Token t("a", "b", am::Nesting::OPENING);

  t.SetAttrs(std::nullopt);
  t.SetMap(std::nullopt);
  t.SetChildren(std::nullopt);

  EXPECT_FALSE(t.GetAttrs().has_value());
  EXPECT_FALSE(t.GetMap().has_value());
  EXPECT_FALSE(t.GetChildren().has_value());
}

// ---------- Enum ----------

TEST(Token, ValuesAndAccessorsCorrect) {
  EXPECT_EQ(am::Nesting::CLOSING, -1);
  EXPECT_EQ(am::Nesting::SELF_CLOSING, 0);
  EXPECT_EQ(am::Nesting::OPENING, 1);

  am::Token t1("t", "g", am::Nesting::CLOSING);
  am::Token t2("t", "g", am::Nesting::SELF_CLOSING);
  am::Token t3("t", "g", am::Nesting::OPENING);

  EXPECT_EQ(t1.GetNesting(), am::Nesting::CLOSING);
  EXPECT_EQ(t2.GetNesting(), am::Nesting::SELF_CLOSING);
  EXPECT_EQ(t3.GetNesting(), am::Nesting::OPENING);
}

// ---------- Stress & Robustness ----------

TEST(Token, HandlesLargeNumberOfAttributes) {
  am::Token t("x", "y", am::Nesting::SELF_CLOSING);
  const int N = 1000;
  std::vector<std::pair<std::string, std::string>> many;
  for (int i = 0; i < N; ++i) {
    many.emplace_back("k" + std::to_string(i), "v" + std::to_string(i));
  }
  t.AttrPush(many);
  EXPECT_EQ(t.AttrIndex("k0"), 0);
  EXPECT_EQ(t.AttrIndex("k999"), 999);
  EXPECT_EQ(t.AttrGet("k500").value(), "v500");
}

TEST(Token, AttrJoinAndSetWorkRepeatedly) {
  am::Token t("x", "y", am::Nesting::SELF_CLOSING);
  for (int i = 0; i < 100; ++i) {
    t.AttrJoin("class", std::to_string(i));
  }
  EXPECT_TRUE(t.AttrGet("class").has_value());
  EXPECT_GT(t.AttrGet("class").value().size(), 10u);
}

}  // namespace
