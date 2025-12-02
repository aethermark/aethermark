// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_block/state_block.hpp"

#include <algorithm>
#include <any>
#include <deque>
#include <string>
#include <utility>
#include <vector>

namespace aethermark {

// NOLINTBEGIN(whitespace/indent_namespace)

StateBlock::StateBlock(const std::string& src_, Aethermark& md_, std::any env_,
                       std::deque<Token>& tokens_)
    : src(src_), md(md_), env(env_), tokens(tokens_) {
  // NOLINTEND
  size_t estimated_lines = std::count(src.begin(), src.end(), '\n') + 4;

  bMarks.reserve(estimated_lines);
  eMarks.reserve(estimated_lines);
  tShift.reserve(estimated_lines);
  sCount.reserve(estimated_lines);
  bsCount.reserve(estimated_lines);

  const std::string& s = src;
  int len = static_cast<int>(s.length());

  int start = 0;
  int pos = 0;
  int indent = 0;
  int offset = 0;
  bool indent_found = false;

  while (pos < len) {
    unsigned char ch = s[pos];

    if (!indent_found) {
      if (isSpace(ch)) {
        indent++;

        if (ch == 0x09) {
          offset += 4 - (offset % 4);
        } else {
          offset++;
        }
        pos++;
        continue;
      } else {
        indent_found = true;
      }
    }

    if (ch == 0x0A || pos == len - 1) {
      if (ch != 0x0A) {
        pos++;
      }

      bMarks.push_back(start);
      eMarks.push_back(pos);
      tShift.push_back(indent);
      sCount.push_back(offset);
      bsCount.push_back(0);

      indent_found = false;
      indent = 0;
      offset = 0;
      start = pos + 1;
    }

    pos++;
  }

  bMarks.push_back(len);
  eMarks.push_back(len);
  tShift.push_back(0);
  sCount.push_back(0);
  bsCount.push_back(0);

  lineMax = static_cast<int>(bMarks.size()) - 1;
}

// NOLINTBEGIN(whitespace/indent_namespace)

Token& StateBlock::push(const std::string& type, const std::string& tag,
                        Nesting nesting) {
  // NOLINTEND

  tokens.emplace_back(type, tag, nesting);
  Token& token = tokens.back();
  token.block = true;

  // Handle nesting
  if (static_cast<int>(nesting) < 0) {
    // closing tag -> decrease level before assigning
    level--;
  }

  token.level = level;

  if (static_cast<int>(nesting) > 0) {
    // opening tag -> increase level after assigning
    level++;
  }

  return token;
}

bool StateBlock::isEmpty(int line) const {
  return bMarks[line] + tShift[line] >= eMarks[line];
}

int StateBlock::skipEmptyLines(int from) const {
  for (int max = lineMax; from < max; from++) {
    if (bMarks[from] + tShift[from] < eMarks[from]) {
      break;
    }
  }
  return from;
}

int StateBlock::skipSpaces(int pos) const {
  int max = src.size();
  while (pos < max) {
    unsigned char ch = src[pos];
    if (!isSpace(ch)) break;
    pos++;
  }
  return pos;
}

int StateBlock::skipSpacesBack(int pos, int min) const {
  if (pos <= min) return pos;

  while (pos > min) {
    unsigned char ch = src[pos - 1];
    if (!isSpace(ch)) return pos;
    pos--;
  }
  return pos;
}

int StateBlock::skipChars(int pos, int code) const {
  int max = src.size();
  while (pos < max) {
    if ((unsigned char)src[pos] != code) break;
    pos++;
  }
  return pos;
}

int StateBlock::skipCharsBack(int pos, int code, int min) const {
  if (pos <= min) return pos;

  while (pos > min) {
    if ((unsigned char)src[pos - 1] != code) return pos;
    pos--;
  }
  return pos;
}

// NOLINTBEGIN(whitespace/indent_namespace)

std::string StateBlock::getLines(int begin, int end, int indent,
                                 bool keepLastLF) const {
  // NOLINTEND
  if (begin >= end) return "";

  std::vector<std::string> out;
  out.reserve(end - begin);

  for (int line = begin; line < end; line++) {
    int lineIndent = 0;
    int lineStart = bMarks[line];
    int first = lineStart;
    int last;

    if (line + 1 < end || keepLastLF) {
      last = eMarks[line] + 1;  // include LF
    } else {
      last = eMarks[line];  // exclude LF
    }

    // compute indent mask
    while (first < last && lineIndent < indent) {
      unsigned char ch = src[first];

      if (isSpace(ch)) {
        if (ch == 0x09) {
          // tab expansion respects bsCount (blockquote hacks)
          lineIndent += 4 - (lineIndent + bsCount[line]) % 4;
        } else {
          lineIndent++;
        }
      } else if (first - lineStart < tShift[line]) {
        // masked region (blockquote markers etc)
        lineIndent++;
      } else {
        break;
      }
      first++;
    }

    if (lineIndent > indent) {
      // partial tab expansion
      int extra = lineIndent - indent;
      out.emplace_back(std::string(extra, ' ') +
                       src.substr(first, last - first));
    } else {
      out.emplace_back(src.substr(first, last - first));
    }
  }

  std::string result;
  result.reserve(64 * (end - begin));  // speed

  for (auto& s : out) result += s;

  return result;
}

}  // namespace aethermark
