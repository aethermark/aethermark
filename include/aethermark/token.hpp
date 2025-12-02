// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <any>
#include <deque>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace aethermark {

/// @brief Represents the type of nesting type of a token.
enum Nesting {
  CLOSING = -1,      ///< Represents a closing token.
  SELF_CLOSING = 0,  ///< Represents a self-closing token.
  OPENING = 1,       ///< Represents an opening token.
};

/// @brief Represents a token in the Aethermark markdown flavor.
class Token {
 public:
  Token() = default;
  Token(const Token&) = default;
  Token(Token&&) noexcept = default;
  Token& operator=(const Token&) = default;
  Token& operator=(Token&&) noexcept = default;
  ~Token() = default;

  /// @brief Constructs a new Token.
  /// @param type Type of the token.
  /// @param tag Tag of the token.
  /// @param nesting Nesting type of the token.
  Token(std::string type, std::string tag, Nesting nesting);

  /// @brief Get the attribute index by name.
  /// @param name Name of the attribute.
  /// @return Index of the attribute.
  int AttrIndex(const std::string& name);

  /// @brief Push attributes to the token.
  /// @param attrData Attribute data to be pushed. ( name, value ) pairs.
  void AttrPush(
      const std::vector<std::pair<std::string, std::string>>& attrData);

  /// @brief Set an attribute to the token. Override old value if exists.
  /// @param name Name of attribute.
  /// @param value Value of attribute.
  void AttrSet(const std::string& name, const std::string& value);

  /// @brief Get the value of attribute by name, or null if it does not exist
  /// @param name Name of the attribute.
  /// @return Value of the attribute or null.
  std::optional<std::string> AttrGet(const std::string& name);

  /// @brief Join value to existing attribute via space.
  /// Or create new attribute if not exists. Useful to operate with token
  /// classes.
  /// @param name Name of the attribute.
  /// @param value Value of the attribute.
  void AttrJoin(const std::string& name, const std::string& value);

  /// @brief Type of the token, e.g. "paragraph_open".
  std::string type;

  /// @brief Tag of the token, e.g. "p".
  std::string tag;

  /// @brief HTML attributes. Format: `[ [ name1, value1 ], [ name2, value2 ]
  /// ]`.
  std::optional<std::vector<std::pair<std::string, std::string>>> attrs;

  /// @brief Source map info. Format: `[ line_begin, line_end ]`
  std::optional<std::pair<float, float>> map;

  /// @brief Level change in the token.
  Nesting nesting;

  /// @brief Nesting level, the same as `state.level`.
  float level;

  /// @brief An array of child nodes (inline and img tokens)
  std::optional<std::deque<Token>> children;

  /// @brief Contents of the tag. If it is a self-closing tag (code, html,
  /// fence, etc.)
  std::string content;

  /// @brief '*' or '_' for emphasis, fence string for fence, etc.
  std::string markup;

  /// @brief One of 3 things:
  /// - Info string for "fence" tokens
  /// - The value "auto" for autolink "link_open" and "link_close" tokens
  /// - The string value of the item marker for ordered-list "list_item_open"
  /// tokens
  std::string info;

  /// @brief A place for plugins to store an arbitrary data.
  std::any meta;

  /// @brief True for block-level tokens, false for inline tokens.
  /// Used in renderer to calculate line breaks.
  bool block;

  /// @brief If it's true, ignore this element when rendering. Used for tight
  /// lists to hide paragraphs.
  bool hidden;
};

}  // namespace aethermark
