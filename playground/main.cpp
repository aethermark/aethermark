// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include <algorithm>
#include <any>
#include <deque>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/parser_block.hpp"
#include "aethermark/token.hpp"

using TableRow = std::vector<std::string>;
using TableData = std::vector<TableRow>;

inline void PrintTable(const TableData& data, const TableRow& headers);
inline void Print(const std::deque<aethermark::Token>& tokens);
inline std::string Repeat(const std::string& str, size_t times);
inline std::string Center(const std::string& str, size_t width);
inline std::string Normalize(std::string s);

int main() {
  std::string src = "    some code";
  aethermark::Aethermark md;
  std::any env;

  std::deque<aethermark::Token> tokens;

  aethermark::StateCore state = aethermark::StateCore(src, md, env);

  md.core_parser.Process(state);

  // md.blockParser.parse(src, md, env, tokens);

  Print(state.tokens);

  return 0;
}

inline void Print(const std::deque<aethermark::Token>& tokens) {
  TableData rows;
  TableRow headers = {
      "Index", "Type", "Content", "Tag", "Map", "Nesting",
  };

  for (size_t i = 0; i < tokens.size(); ++i) {
    auto& token = tokens[i];
    TableRow row;

    row.push_back(std::to_string(i));
    row.push_back(token.type);
    row.push_back(token.content != "" ? Normalize(token.content) : "-");
    row.push_back(token.tag);

    if (token.map) {
      std::string map_str =
          "[" + std::to_string(static_cast<int>(token.map->first)) + ", " +
          std::to_string(static_cast<int>(token.map->second)) + "] ";
      row.push_back(map_str);
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

  PrintTable(rows, headers);
}

inline void PrintTable(const TableData& data, const TableRow& headers) {
  if (data.empty() && headers.empty()) return;

  size_t cols = headers.size();
  std::vector<size_t> widths(cols, 0);

  // compute column widths
  for (size_t i = 0; i < cols; i++) {
    widths[i] = headers[i].size();
  }

  for (auto& row : data) {
    for (size_t i = 0; i < row.size(); i++) {
      widths[i] = std::max(widths[i], row[i].size());
    }
  }

  // Colors and formatting
  const std::string blue = "\033[34m";
  const std::string green = "\033[32m";
  const std::string red = "\033[31m";
  const std::string bold = "\033[1m";
  const std::string reset = "\033[0m";

  // Helper to print horizontal line
  auto line = [&](const std::string& left, const std::string& mid,
                  const std::string& right) {
    std::cout << left;
    for (size_t i = 0; i < cols; i++) {
      std::cout << Repeat("─", widths[i] + 2);
      std::cout << (i < cols - 1 ? mid : right);
    }
    std::cout << "\n";
  };

  // TOP border
  line("┌", "┬", "┐");

  // HEADER
  std::cout << "│";
  for (size_t i = 0; i < cols; i++) {
    std::cout << " " << bold << blue << std::setw(widths[i])
              << Center(headers[i], widths[i]) << reset << " │";
  }
  std::cout << "\n";

  // MID border
  line("├", "┼", "┤");

  // DATA rows
  for (auto& row : data) {
    std::cout << "│";
    for (size_t i = 0; i < cols; i++) {
      std::string val = (i < row.size()) ? row[i] : "";
      std::string color = (val == "-") ? red : green;
      std::cout << " " << color << std::setw(widths[i]) << std::left << val
                << reset << " │";
    }
    std::cout << "\n";
  }

  // BOTTOM border
  line("└", "┴", "┘");
}

inline std::string Repeat(const std::string& str, size_t times) {
  std::string result;
  for (size_t i = 0; i < times; ++i) {
    result += str;
  }
  return result;
}

inline std::string Center(const std::string& str, size_t width) {
  if (str.size() >= width) return str;
  size_t pad = width - str.size();
  size_t pad_left = pad / 2;
  size_t pad_right = pad - pad_left;
  return std::string(pad_left, ' ') + str + std::string(pad_right, ' ');
}

inline std::string Normalize(std::string s) {
  const std::regex kNewlineRe(R"(\r\n?|\n)");
  const std::regex kNullRe(std::string("\x00", 1));

  return std::regex_replace(s, std::regex(R"(\r\n?|\n)"), "\\n");
}
