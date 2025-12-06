// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include <algorithm>
#include <any>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/token.hpp"

using TableRow = std::vector<std::string>;
using TableData = std::vector<TableRow>;

inline void Print(const std::deque<aethermark::Token>& tokens);
inline std::string Repeat(const std::string& str, size_t times);
inline std::string Center(const std::string& s, size_t w);
inline std::string Normalize(const std::string& s);

int main(int argc, char** argv) {
  std::string input;

  if (argc > 1) {
    std::ifstream file(argv[1]);
    if (!file) {
      std::cerr << "Failed to open file: " << argv[1] << "\n";
      return 1;
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    input = oss.str();
  } else {
    std::ostringstream oss;
    for (int i = 1; i < argc; i++) {
      if (i > 1) oss << " ";
      oss << argv[i];
    }
    input = oss.str();
  }

  aethermark::Aethermark am("commonmark");
  Print(am.Parse(input, {}));
  return 0;
}

inline void Print(const std::deque<aethermark::Token>& tokens) {
  TableData rows;
  TableRow headers = {"Index", "Type", "Content", "Tag", "Map", "Nesting"};

  for (size_t i = 0; i < tokens.size(); ++i) {
    const auto& token = tokens[i];
    TableRow row;
    row.push_back(std::to_string(i));
    row.push_back(token.type);
    row.push_back(token.content.empty() ? "-" : Normalize(token.content));
    row.push_back(token.tag);

    if (token.map) {
      row.push_back("[" + std::to_string(token.map->first) + ", " +
                    std::to_string(token.map->second) + "]");
    } else {
      row.push_back("-");
    }

    std::string nesting;
    switch (token.nesting) {
      case aethermark::Nesting::kOpening:
        nesting = "OPENING";
        break;
      case aethermark::Nesting::kClosing:
        nesting = "CLOSING";
        break;
      case aethermark::Nesting::kSelfClosing:
        nesting = "SELF_CLOSING";
        break;
      default:
        nesting = "UNKNOWN";
        break;
    }
    row.push_back(nesting);
    rows.push_back(row);
  }

  if (rows.empty()) return;

  size_t cols = headers.size();
  std::vector<size_t> widths(cols, 0);
  for (size_t i = 0; i < cols; i++) widths[i] = headers[i].size();
  for (auto& row : rows)
    for (size_t i = 0; i < row.size(); i++)
      widths[i] = std::max(widths[i], row[i].size());

  const std::string blue = "\033[34m";
  const std::string green = "\033[32m";
  const std::string red = "\033[31m";
  const std::string bold = "\033[1m";
  const std::string reset = "\033[0m";

  auto line = [&](const std::string& left, const std::string& mid,
                  const std::string& right) {
    std::cout << left;
    for (size_t i = 0; i < cols; i++) {
      std::cout << Repeat("─", widths[i] + 2);
      std::cout << (i < cols - 1 ? mid : right);
    }
    std::cout << "\n";
  };

  line("┌", "┬", "┐");

  // header
  std::cout << "│";
  for (size_t i = 0; i < cols; i++) {
    std::cout << " " << bold << blue << Center(headers[i], widths[i]) << reset
              << " │";
  }
  std::cout << "\n";
  line("├", "┼", "┤");

  // rows
  for (auto& row : rows) {
    std::cout << "│";
    for (size_t i = 0; i < cols; i++) {
      std::string val = (i < row.size()) ? row[i] : "";
      std::string color = (val == "-") ? red : green;
      std::cout << " " << color << std::left << std::setw(widths[i]) << val
                << reset << " │";
    }
    std::cout << "\n";
  }

  line("└", "┴", "┘");
}

inline std::string Repeat(const std::string& str, size_t times) {
  std::string result;
  result.reserve(str.size() * times);
  for (size_t i = 0; i < times; ++i) result += str;
  return result;
}

inline std::string Center(const std::string& s, size_t w) {
  if (s.size() >= w) return s;
  size_t pad_left = (w - s.size()) / 2;
  size_t pad_right = w - s.size() - pad_left;
  return std::string(pad_left, ' ') + s + std::string(pad_right, ' ');
}

inline std::string Normalize(const std::string& s) {
  return std::regex_replace(s, std::regex(R"(\r\n?|\n)"), "\\n");
}
