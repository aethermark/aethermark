// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include <algorithm>
#include <any>
#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/parser_block.hpp"
#include "aethermark/token.hpp"

using TableRow = std::vector<std::string>;
using TableData = std::vector<TableRow>;

inline void printTable(const TableData& data, const TableRow& headers);
inline void print(const std::deque<aethermark::Token>& tokens);
inline std::string repeat(const std::string& str, size_t times);
inline std::string center(const std::string& str, size_t width);

int main() {
  std::string src = "> Blockquotes can also be nested...";
  aethermark::Aethermark md;
  std::any env;

  std::deque<aethermark::Token> tokens;

  md.blockParser.parse(src, md, env, tokens);

  print(tokens);

  return 0;
}

inline void print(const std::deque<aethermark::Token>& tokens) {
  TableData rows;
  TableRow headers = {
      "Index", "Type", "Content", "Tag", "Map", "Nesting",
  };

  for (size_t i = 0; i < tokens.size(); ++i) {
    auto& token = tokens[i];
    TableRow row;

    row.push_back(std::to_string(i));
    row.push_back(token.type);
    row.push_back(token.content != "" ? token.content : "-");
    row.push_back(token.tag);

    if (token.map) {
      std::string mapStr =
          "[" + std::to_string(static_cast<int>(token.map->first)) + ", " +
          std::to_string(static_cast<int>(token.map->second)) + "] ";
      row.push_back(mapStr);
    } else {
      row.push_back("-");
    }

    std::string nesting;

    switch (token.nesting) {
      case aethermark::Nesting::OPENING:
        nesting = "OPENING";
        break;
      case aethermark::Nesting::CLOSING:
        nesting = "CLOSING";
        break;
      case aethermark::Nesting::SELF_CLOSING:
        nesting = "SELF_CLOSING";
        break;
      default:
        nesting = "UNKNOWN";
        break;
    }
    row.push_back(nesting);

    rows.push_back(row);
  }

  printTable(rows, headers);
}

inline void printTable(const TableData& data, const TableRow& headers) {
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
      std::cout << repeat(u8"─", widths[i] + 2);
      std::cout << (i < cols - 1 ? mid : right);
    }
    std::cout << "\n";
  };

  // TOP border
  line(u8"┌", u8"┬", u8"┐");

  // HEADER
  std::cout << "│";
  for (size_t i = 0; i < cols; i++) {
    std::cout << " " << bold << blue << std::setw(widths[i])
              << center(headers[i], widths[i]) << reset << " │";
  }
  std::cout << "\n";

  // MID border
  line(u8"├", u8"┼", u8"┤");

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
  line(u8"└", u8"┴", u8"┘");
}

inline std::string repeat(const std::string& str, size_t times) {
  std::string result;
  for (size_t i = 0; i < times; ++i) {
    result += str;
  }
  return result;
}

inline std::string center(const std::string& str, size_t width) {
  if (str.size() >= width) return str;
  size_t pad = width - str.size();
  size_t pad_left = pad / 2;
  size_t pad_right = pad - pad_left;
  return std::string(pad_left, ' ') + str + std::string(pad_right, ' ');
}
