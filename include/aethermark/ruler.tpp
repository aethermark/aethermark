// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "ruler.hpp"

namespace aethermark {

template <typename T>
Rule<T>* Ruler<T>::findRule(const std::string& name) {
  for (auto& r : rules_) {
    if (r.name == name) return &r;
  }
  return nullptr;
}

template <typename T>
const Rule<T>* Ruler<T>::findRule(const std::string& name) const {
  for (const auto& r : rules_) {
    if (r.name == name) return &r;
  }
  return nullptr;
}

template <typename T>
void Ruler<T>::compile() const {
  std::vector<std::string> chains;
  chains.push_back("");  // default chain

  // collect unique alt-chains
  for (const auto& r : rules_) {
    if (!r.enabled) continue;
    for (const auto& alt : r.alt) {
      if (std::find(chains.begin(), chains.end(), alt) == chains.end()) {
        chains.push_back(alt);
      }
    }
  }

  cache_.clear();

  // build cache for each chain
  for (const auto& chain : chains) {
    std::vector<T> seq;

    for (const auto& r : rules_) {
      if (!r.enabled) continue;

      if (!chain.empty()) {
        // if chain != "" then rule must include this chain
        if (std::find(r.alt.begin(), r.alt.end(), chain) == r.alt.end())
          continue;
      }

      seq.push_back(r.fn);
    }

    cache_[chain] = std::move(seq);
  }
}

template <typename T>
void Ruler<T>::at(const std::string& name, T fn, const RuleOptions& options) {
  Rule<T>* r = findRule(name);
  if (!r) throw std::runtime_error("Parser rule not found: " + name);

  r->fn = fn;
  r->alt = options.alt;

  touch();
}

template <typename T>
void Ruler<T>::before(const std::string& beforeName,
                      const std::string& ruleName, T fn,
                      const RuleOptions& options) {
  Rule<T>* target = findRule(beforeName);
  if (!target) throw std::runtime_error("Parser rule not found: " + beforeName);

  // find index
  auto it = std::find_if(rules_.begin(), rules_.end(), [&](const Rule<T>& r) {
    return r.name == beforeName;
  });

  rules_.insert(it, Rule<T>{ruleName, fn, options.alt, true});

  touch();
}

template <typename T>
void Ruler<T>::after(const std::string& afterName, const std::string& ruleName,
                     T fn, const RuleOptions& options) {
  Rule<T>* target = findRule(afterName);
  if (!target) throw std::runtime_error("Parser rule not found: " + afterName);

  auto it = std::find_if(rules_.begin(), rules_.end(),
                         [&](const Rule<T>& r) { return r.name == afterName; });

  rules_.insert(it + 1, Rule<T>{ruleName, fn, options.alt, true});

  touch();
}

template <typename T>
void Ruler<T>::push(const std::string& ruleName, T fn,
                    const RuleOptions& options) {
  rules_.push_back(Rule<T>{ruleName, fn, options.alt, true});
  touch();
}

template <typename T>
std::vector<std::string> Ruler<T>::enable(const std::vector<std::string>& list,
                                          bool ignoreInvalid) {
  std::vector<std::string> res;

  for (auto& name : list) {
    Rule<T>* r = findRule(name);
    if (!r) {
      if (ignoreInvalid) continue;
      throw std::runtime_error("Invalid rule name: " + name);
    }
    r->enabled = true;
    res.push_back(name);
  }

  touch();
  return res;
}

template <typename T>
std::vector<std::string> Ruler<T>::enable(const std::string& name,
                                          bool ignoreInvalid) {
  return enable(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
void Ruler<T>::enableOnly(const std::vector<std::string>& list,
                          bool ignoreInvalid) {
  // disable everything
  for (auto& r : rules_) r.enabled = false;

  // then enable only these
  enable(list, ignoreInvalid);
}

template <typename T>
std::vector<std::string> Ruler<T>::disable(const std::vector<std::string>& list,
                                           bool ignoreInvalid) {
  std::vector<std::string> res;

  for (auto& name : list) {
    Rule<T>* r = findRule(name);
    if (!r) {
      if (ignoreInvalid) continue;
      throw std::runtime_error("Invalid rule name: " + name);
    }
    r->enabled = false;
    res.push_back(name);
  }

  touch();
  return res;
}

template <typename T>
std::vector<std::string> Ruler<T>::disable(const std::string& name,
                                           bool ignoreInvalid) {
  return disable(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
std::vector<T> Ruler<T>::getRules(const std::string& chainName) const {
  if (cache_.empty()) compile();

  auto it = cache_.find(chainName);
  if (it == cache_.end()) return {};

  return it->second;
}

}  // namespace aethermark
