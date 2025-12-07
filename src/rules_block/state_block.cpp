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

StateBlock::StateBlock(const std::string& src_, Aethermark& md_, std::any env_,
                       std::deque<Token>& tokens_)
    : src(src_), md(md_), env(env_), tokens(tokens_) {
  size_t estimated_lines = std::count(src.begin(), src.end(), '\n') + 4;

  b_marks.reserve(estimated_lines);
  e_marks.reserve(estimated_lines);
  t_shift.reserve(estimated_lines);
  s_count.reserve(estimated_lines);
  bs_count.reserve(estimated_lines);

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
      if (Utils::IsSpace(ch)) {
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

      b_marks.push_back(start);
      e_marks.push_back(pos);
      t_shift.push_back(indent);
      s_count.push_back(offset);
      bs_count.push_back(0);

      indent_found = false;
      indent = 0;
      offset = 0;
      start = pos + 1;
    }

    pos++;
  }

  b_marks.push_back(len);
  e_marks.push_back(len);
  t_shift.push_back(0);
  s_count.push_back(0);
  bs_count.push_back(0);

  line_max = static_cast<int>(b_marks.size()) - 1;
}

Token& StateBlock::Push(const std::string& type, const std::string& tag,
                        Nesting nesting) {
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

bool StateBlock::IsEmpty(int line) const {
  return b_marks[line] + t_shift[line] >= e_marks[line];
}

int StateBlock::SkipEmptyLines(int from) const {
  for (int max = line_max; from < max; from++) {
    if (b_marks[from] + t_shift[from] < e_marks[from]) {
      break;
    }
  }
  return from;
}

int StateBlock::SkipSpaces(int pos) const {
  int max = src.size();
  while (pos < max) {
    unsigned char ch = src[pos];
    if (!Utils::IsSpace(ch)) break;
    pos++;
  }
  return pos;
}

int StateBlock::SkipSpacesBack(int pos, int min) const {
  if (pos <= min) return pos;

  while (pos > min) {
    unsigned char ch = src[pos - 1];
    if (!Utils::IsSpace(ch)) return pos;
    pos--;
  }
  return pos;
}

int StateBlock::SkipChars(int pos, int code) const {
  int max = src.size();
  while (pos < max) {
    if ((unsigned char)src[pos] != code) break;
    pos++;
  }
  return pos;
}

int StateBlock::SkipCharsBack(int pos, int code, int min) const {
  if (pos <= min) return pos;

  while (pos > min) {
    if ((unsigned char)src[pos - 1] != code) return pos;
    pos--;
  }
  return pos;
}

std::string StateBlock::GetLines(int begin, int end, int indent,
                                 bool keepLastLF) const {
  if (begin >= end) return "";

  std::vector<std::string> out;
  out.reserve(end - begin);

  for (int line = begin; line < end; line++) {
    int line_indent = 0;
    int line_start = b_marks[line];
    int first = line_start;
    int last;

    if (line + 1 < end || keepLastLF) {
      last = e_marks[line] + 1;  // include LF
    } else {
      last = e_marks[line];  // exclude LF
    }

    // compute indent mask
    while (first < last && line_indent < indent) {
      unsigned char ch = src[first];

      if (Utils::IsSpace(ch)) {
        if (ch == 0x09) {
          // tab expansion respects bs_count (blockquote hacks)
          line_indent += 4 - (line_indent + bs_count[line]) % 4;
        } else {
          line_indent++;
        }
      } else if (first - line_start < t_shift[line]) {
        // masked region (blockquote markers etc)
        line_indent++;
      } else {
        break;
      }
      first++;
    }

    if (line_indent > indent) {
      // partial tab expansion
      int extra = line_indent - indent;
      out.emplace_back(std::string(extra, ' ') +
                       src.substr(first, last - first));
    } else {
      out.emplace_back(src.substr(first, last - first));
    }
  }

  std::string result;
  result.reserve(64 * (end - begin));  // speed

  for (std::string& s : out) result += s;

  return result;
}

}  // namespace aethermark
