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
Rule<T>* Ruler<T>::FindRule(const std::string& name) {
  for (Rule<T>& r : rules_) {
    if (r.name == name) return &r;
  }
  return nullptr;
}

template <typename T>
const Rule<T>* Ruler<T>::FindRule(const std::string& name) const {
  for (const Rule<T>& r : rules_) {
    if (r.name == name) return &r;
  }
  return nullptr;
}

template <typename T>
void Ruler<T>::Compile() const {
  std::vector<std::string> chains;
  chains.push_back("");  // default chain

  // collect unique alt-chains
  for (const Rule<T>& r : rules_) {
    if (!r.enabled) continue;
    for (const std::string& alt : r.alt) {
      if (std::find(chains.begin(), chains.end(), alt) == chains.end()) {
        chains.push_back(alt);
      }
    }
  }

  cache_.clear();

  // build cache for each chain
  for (const std::string& chain : chains) {
    std::vector<T> seq;

    for (const Rule<T>& r : rules_) {
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
void Ruler<T>::At(const std::string& name, T fn, const RuleOptions& options) {
  Rule<T>* r = FindRule(name);
  if (!r) throw std::runtime_error("Parser rule not found: " + name);

  r->fn = fn;
  r->alt = options.alt;

  Touch();
}

template <typename T>
void Ruler<T>::Before(const std::string& beforeName,
                      const std::string& ruleName, T fn,
                      const RuleOptions& options) {
  Rule<T>* target = FindRule(beforeName);
  if (!target) throw std::runtime_error("Parser rule not found: " + beforeName);

  // find index
  auto it = std::find_if(rules_.begin(), rules_.end(), [&](const Rule<T>& r) {
    return r.name == beforeName;
  });

  rules_.insert(it, Rule<T>{ruleName, fn, options.alt, true});

  Touch();
}

template <typename T>
void Ruler<T>::After(const std::string& afterName, const std::string& ruleName,
                     T fn, const RuleOptions& options) {
  Rule<T>* target = FindRule(afterName);
  if (!target) throw std::runtime_error("Parser rule not found: " + afterName);

  auto it = std::find_if(rules_.begin(), rules_.end(),
                         [&](const Rule<T>& r) { return r.name == afterName; });

  rules_.insert(it + 1, Rule<T>{ruleName, fn, options.alt, true});

  Touch();
}

template <typename T>
void Ruler<T>::Push(const std::string& ruleName, T fn,
                    const RuleOptions& options) {
  rules_.push_back(Rule<T>{ruleName, fn, options.alt, true});
  Touch();
}

template <typename T>
std::vector<std::string> Ruler<T>::Enable(const std::vector<std::string>& list,
                                          bool ignoreInvalid) {
  std::vector<std::string> res;

  for (auto& name : list) {
    Rule<T>* r = FindRule(name);
    if (!r) {
      if (ignoreInvalid) continue;
      throw std::runtime_error("Invalid rule name: " + name);
    }
    r->enabled = true;
    res.push_back(name);
  }

  Touch();
  return res;
}

template <typename T>
std::vector<std::string> Ruler<T>::Enable(const std::string& name,
                                          bool ignoreInvalid) {
  return Enable(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
void Ruler<T>::EnableOnly(const std::vector<std::string>& list,
                          bool ignoreInvalid) {
  // disable everything
  for (auto& r : rules_) r.enabled = false;

  // then enable only these
  Enable(list, ignoreInvalid);
}

template <typename T>
std::vector<std::string> Ruler<T>::Disable(const std::vector<std::string>& list,
                                           bool ignoreInvalid) {
  std::vector<std::string> res;

  for (auto& name : list) {
    Rule<T>* r = FindRule(name);
    if (!r) {
      if (ignoreInvalid) continue;
      throw std::runtime_error("Invalid rule name: " + name);
    }
    r->enabled = false;
    res.push_back(name);
  }

  Touch();
  return res;
}

template <typename T>
std::vector<std::string> Ruler<T>::Disable(const std::string& name,
                                           bool ignoreInvalid) {
  return Disable(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
std::vector<T> Ruler<T>::GetRules(const std::string& chainName) const {
  if (cache_.empty()) Compile();

  auto it = cache_.find(chainName);
  if (it == cache_.end()) return {};

  return it->second;
}

}  // namespace aethermark
