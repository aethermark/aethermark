// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <any>
#include <string>
#include <vector>

// #include "aethermark/aethermark.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

class Aethermark;

enum class ParentType { Blockquote, List, Root, Paragraph, Reference };

class StateBlock {
 public:
  // NOLINTBEGIN(whitespace/indent_namespace)

  static inline bool isSpace(int ch) { return ch == 0x20 || ch == 0x09; }

  StateBlock(const std::string& src,
             Aethermark& md,  // NOLINT(runtime/references)
             std::any env,
             std::vector<Token>& tokens);  // NOLINT(runtime/references)
  // NOLINTEND

  std::string src;
  Aethermark& md;
  std::any env;

  // tokens stream (block-level)
  std::vector<Token>& tokens;

  // line metadata
  std::vector<int> bMarks;   // beginning offsets
  std::vector<int> eMarks;   // ending offsets
  std::vector<int> tShift;   // first non-space char (tabs not expanded)
  std::vector<int> sCount;   // indent count (tabs expanded)
  std::vector<int> bsCount;  // special tab-expansion offset (blockquotes hack)

  // parser state vars
  int blkIndent = 0;
  int line = 0;
  int lineMax = 0;
  bool tight = false;
  int ddIndent = -1;
  int listIndent = -1;

  ParentType parentType = ParentType::Root;

  int level = 0;

  // methods
  Token& push(const std::string& type, const std::string& tag, Nesting nesting);

  bool isEmpty(int line) const;

  int skipEmptyLines(int from) const;

  int skipSpaces(int pos) const;

  int skipSpacesBack(int pos, int min) const;

  int skipChars(int pos, int code) const;

  int skipCharsBack(int pos, int code, int min) const;

  std::string getLines(int begin, int end, int indent, bool keepLastLF) const;

  using TokenType = Token;
};

}  // namespace aethermark
