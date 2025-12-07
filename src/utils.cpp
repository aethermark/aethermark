// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/utils.hpp"

#include <string>

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

}  // namespace aethermark
