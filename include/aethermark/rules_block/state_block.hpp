// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <any>
#include <deque>
#include <string>
#include <vector>

#include "aethermark/token.hpp"
#include "aethermark/utils.hpp"

namespace aethermark {

class Aethermark;

enum class ParentType { Blockquote, List, Root, Paragraph, Reference };

class StateBlock {
 public:
  /// @brief Constructs a new block state object.
  /// @param src Document source.
  /// @param md Reference to the parser.
  /// @param env Environment for parser.
  /// @param tokens List of tokens.
  StateBlock(const std::string& src,
             Aethermark& md,  // NOLINT(runtime/references)
             std::any env,
             std::deque<Token>& tokens);  // NOLINT(runtime/references)

  /// @brief Source string.
  std::string src;

  /// @brief Reference to the parser.
  Aethermark& md;

  /// @brief Environment for parser.
  std::any env;

  /// @brief Token stream (block-level).
  std::deque<Token>& tokens;

  // line metadata

  /// @brief beginning offsets.
  std::vector<int> bMarks;

  /// @brief ending offsets.
  std::vector<int> eMarks;

  /// @brief first non-space char (tabs not expanded).
  std::vector<int> tShift;

  /// @brief indent count (tabs expanded).
  std::vector<int> sCount;

  /// @brief special tab-expansion offset (for blockquotes).
  std::vector<int> bsCount;

  // parser state vars

  /// @brief Indent of current block.
  int blkIndent = 0;

  /// @brief Current line number of the line being processed.
  int line = 0;

  /// @brief Number of lines in document.
  int lineMax = 0;

  /// @brief Whether list is tight (no <p>).
  bool tight = false;

  /// @brief Definition list indent.
  int ddIndent = -1;

  /// @brief List indent.
  int listIndent = -1;

  /// @brief Used in lists to determine if they interrupt a paragraph.
  ParentType parentType = ParentType::Root;

  /// @brief Nesting level.
  int level = 0;

  /// @brief Push token to token stream.
  /// @param type Type of token.
  /// @param tag Tag of token.
  /// @param nesting Nesting level of token.
  /// @return Reference to the pushed token.
  Token& push(const std::string& type, const std::string& tag, Nesting nesting);

  /// @brief Whether line is empty.
  /// @param line Index of line.
  bool isEmpty(int line) const;

  /// @brief Skip processing of empty lines.
  /// @param from Index of line from which skipping begins.
  /// @return Index of first non-empty line.
  int skipEmptyLines(int from) const;

  /// @brief Skip spaces from given position.
  /// @param pos index in source.
  /// @return index of first non-space character.
  int skipSpaces(int pos) const;

  /// @brief Skip spaces from given position in reverse.
  /// @param pos Index of position (right).
  /// @param min Index where the skipping will stop (left).
  /// @return New index of the non-space character.
  int skipSpacesBack(int pos, int min) const;

  /// @brief Skip give character from given position.
  /// @param pos Initial index.
  /// @param code Character to be escaped.
  /// @return Index of first non-code character.
  int skipChars(int pos, int code) const;

  /// @brief Skip characters from given position in reverse.
  /// @param pos Initial index (right).
  /// @param code Character to be escaped.
  /// @param min Index where the skipping will stop (left).
  /// @return Index of first non-code character.
  int skipCharsBack(int pos, int code, int min) const;

  /// @brief Cut lines range from source.
  /// @param begin The first line index to include (inclusive).
  /// @param end The line index to stop at (exclusive).
  /// @param indent The amount of indentation to remove from each line.
  /// @param keepLastLF If true, keeps the final newline character of the last
  /// extracted line.
  /// @return A single string containing the concatenated lines.
  std::string getLines(int begin, int end, int indent, bool keepLastLF) const;

  using TokenType = Token;
};

}  // namespace aethermark
