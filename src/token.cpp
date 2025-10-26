// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/token.hpp"

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace aethermark {

Token::Token(std::string type, std::string tag, Nesting nesting) {
  this->type = type;
  this->tag = tag;
  this->nesting = nesting;
  this->attrs = std::nullopt;
  this->map = std::nullopt;
  this->level = 0;
  this->children = std::nullopt;
  this->content = "";
  this->markup = "";
  this->info = "";
  this->block = false;
  this->hidden = false;
}

int Token::AttrIndex(std::string name) {
  // Check null
  if (!this->attrs.has_value()) {
    return -1;
  }
  // Search for attribute
  for (size_t i = 0; i < this->attrs->size(); i++) {
    if (this->attrs->at(i).first == name) {
      return static_cast<int>(i);
    }
  }
  // Not found
  return -1;
}

using Attr = std::pair<std::string, std::string>;
void Token::AttrPush(std::vector<Attr> attrData) {
  // Initialize attrs if null
  if (!this->attrs.has_value()) {
    this->attrs.emplace();
  }

  // Push attributes
  attrs->insert(attrs->end(), attrData.begin(), attrData.end());
}

void Token::AttrSet(std::string name, std::string value) {
  int index = this->AttrIndex(name);
  if (index == -1) {
    // Add new attribute
    if (!this->attrs.has_value()) {
      this->attrs.emplace();
    }
    this->attrs->emplace_back(std::make_pair(name, value));
    return;
  }
  // Override existing attribute
  for (auto& a : *attrs) {
    if (a.first == name) a.second = value;
  }
}

std::optional<std::string> Token::AttrGet(std::string name) {
  int index = this->AttrIndex(name);
  if (index == -1) return std::nullopt;
  return this->attrs->at(index).second;
}

void Token::AttrJoin(std::string name, std::string value) {
  int index = this->AttrIndex(name);
  if (index == -1) {
    // Add new attribute
    if (!this->attrs.has_value()) {
      this->attrs.emplace();
    }
    this->attrs->emplace_back(std::make_pair(name, value));
    return;
  }
  // Join to existing attribute
  this->attrs->at(index).second += " " + value;
}
}  // namespace aethermark
