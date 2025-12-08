// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/utils.hpp"

#include <algorithm>
#include <string>

#include "aethermark/rules_block/state_block.hpp"

namespace aethermark {

std::string Utils::Trim(const std::string& s) {
  if (s.empty()) return s;

  size_t start = 0;
  while (start < s.size() &&
         std::isspace(static_cast<unsigned char>(s[start]))) {
    start++;
  }

  if (start == s.size()) return "";

  size_t end = s.size() - 1;
  while (end > start && std::isspace(static_cast<unsigned char>(s[end]))) {
    end--;
  }

  return s.substr(start, end - start + 1);
}

bool Utils::IsSpace(int ch) { return ch == 0x20 || ch == 0x09; }

std::string Utils::Slice(const std::string& s, int start, int end) {
  int n = s.size();

  // Handle negative indexes
  if (start < 0) start = n + start;
  if (end < 0) end = n + end;

  // Clamp bounds
  start = std::clamp(start, 0, n);
  end = std::clamp(end, 0, n);

  // If range invalid -> return empty
  if (end <= start) return "";

  return s.substr(start, end - start);
}

int Utils::SkipBulletListMarker(const StateBlock& state, int start_line) {
  const int max = state.e_marks[start_line];
  int pos = state.b_marks[start_line] + state.t_shift[start_line];

  const char marker = state.src[pos++];
  // Check bullet
  if (marker != 0x2A /* * */ && marker != 0x2D /* - */ &&
      marker != 0x2B /* + */) {
    return -1;
  }

  if (pos < max) {
    const char ch = state.src[pos];

    if (!IsSpace(ch)) {  // " -test " - is not a list item
      return -1;
    }
  }

  return pos;
}

int Utils::SkipOrderedListmarker(const StateBlock& state, int start_line) {
  const int start = state.b_marks[start_line] + state.t_shift[start_line];
  const int max = state.e_marks[start_line];
  int pos = start;

  // List marker should have at least 2 chars (digit + dot)
  if (pos + 1 >= max) {
    return -1;
  }

  char ch = state.src[pos++];

  if (ch < 0x30 /* 0 */ || ch > 0x39 /* 9 */) {
    return -1;
  }

  for (;;) {
    // EOL -> fail
    if (pos >= max) {
      return -1;
    }

    ch = state.src[pos++];

    if (ch >= 0x30 /* 0 */ && ch <= 0x39 /* 9 */) {
      // List marker should have no more than 9 digits
      // (prevents integer overflow in browsers)
      if (pos - start >= 10) {
        return -1;
      }

      continue;
    }

    // found valid marker
    if (ch == 0x29 /* ) */ || ch == 0x2e /* . */) {
      break;
    }

    return -1;
  }

  if (pos < max) {
    ch = state.src[pos];

    if (!IsSpace(ch)) {  // " 1.test " - is not a list item
      return -1;
    }
  }
  return pos;
}

void Utils::MarkTightParagraphs(const StateBlock& state, int idx) {
  const int level = state.level + 2;

  for (int i = idx + 2, l = state.tokens.size() - 2; i < l; i++) {
    if (state.tokens[i].level == level &&
        state.tokens[i].type == "paragraph_open") {
      state.tokens[i + 2].hidden = true;
      state.tokens[i].hidden = true;
      i += 2;
    }
  }
}

}  // namespace aethermark
