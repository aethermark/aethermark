#pragma once
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "ruler.hpp"

namespace aethermark {

template <typename T>
int Ruler<T>::find(const std::string& name) const {
  for (std::size_t i = 0; i < rules_.size(); ++i) {
    if (rules_[i].name == name) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

template <typename T>
void Ruler<T>::compile() const {
  // build chains: default '' plus any alt names from enabled rules
  std::unordered_set<std::string> chainSet;
  chainSet.insert(std::string(""));  // default chain

  for (const auto& rule : rules_) {
    if (!rule.enabled) continue;
    for (const auto& altName : rule.alt) {
      chainSet.insert(altName);
    }
  }

  cache_.clear();
  for (const auto& chain : chainSet) {
    std::vector<T> list;
    list.reserve(rules_.size());
    for (const auto& rule : rules_) {
      if (!rule.enabled) continue;
      if (!chain.empty()) {
        // if chain is specified, include only rules that have it in alt
        if (std::find(rule.alt.begin(), rule.alt.end(), chain) ==
            rule.alt.end())
          continue;
      }
      list.push_back(rule.fn);
    }
    cache_.emplace(chain, std::move(list));
  }
}

template <typename T>
void Ruler<T>::at(const std::string& name, const T& fn,
                  const RuleOptions& options) {
  int idx = find(name);
  if (idx == -1) {
    throw std::runtime_error("Parser rule not found: " + name);
  }
  rules_[idx].fn = fn;
  rules_[idx].alt = options.alt;
  cache_.clear();
}

template <typename T>
void Ruler<T>::before(const std::string& beforeName,
                      const std::string& ruleName, const T& fn,
                      const RuleOptions& options) {
  int idx = find(beforeName);
  if (idx == -1) {
    throw std::runtime_error("Parser rule not found: " + beforeName);
  }
  RuleItem<T> item;
  item.name = ruleName;
  item.enabled = true;
  item.fn = fn;
  item.alt = options.alt;
  rules_.insert(rules_.begin() + idx, std::move(item));
  cache_.clear();
}

template <typename T>
void Ruler<T>::after(const std::string& afterName, const std::string& ruleName,
                     const T& fn, const RuleOptions& options) {
  int idx = find(afterName);
  if (idx == -1) {
    throw std::runtime_error("Parser rule not found: " + afterName);
  }
  RuleItem<T> item;
  item.name = ruleName;
  item.enabled = true;
  item.fn = fn;
  item.alt = options.alt;
  rules_.insert(rules_.begin() + idx + 1, std::move(item));
  cache_.clear();
}

template <typename T>
void Ruler<T>::push(const std::string& ruleName, const T& fn,
                    const RuleOptions& options) {
  RuleItem<T> item;
  item.name = ruleName;
  item.enabled = true;
  item.fn = fn;
  item.alt = options.alt;
  rules_.push_back(std::move(item));
  cache_.clear();
}

template <typename T>
std::vector<std::string> Ruler<T>::enable(const std::vector<std::string>& list,
                                          bool ignoreInvalid) {
  std::vector<std::string> result;
  for (const auto& name : list) {
    int idx = find(name);
    if (idx < 0) {
      if (ignoreInvalid) continue;
      throw std::runtime_error("Rules manager: invalid rule name " + name);
    }
    rules_[idx].enabled = true;
    result.push_back(name);
  }
  cache_.clear();
  return result;
}

template <typename T>
std::vector<std::string> Ruler<T>::enable(const std::string& name,
                                          bool ignoreInvalid) {
  return enable(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
void Ruler<T>::enableOnly(const std::vector<std::string>& list,
                          bool ignoreInvalid) {
  // disable all first
  for (auto& rule : rules_) rule.enabled = false;
  // then enable requested
  enable(list, ignoreInvalid);
  cache_.clear();
}

template <typename T>
void Ruler<T>::enableOnly(const std::string& name, bool ignoreInvalid) {
  enableOnly(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
std::vector<std::string> Ruler<T>::disable(const std::vector<std::string>& list,
                                           bool ignoreInvalid) {
  std::vector<std::string> result;
  for (const auto& name : list) {
    int idx = find(name);
    if (idx < 0) {
      if (ignoreInvalid) continue;
      throw std::runtime_error("Rules manager: invalid rule name " + name);
    }
    rules_[idx].enabled = false;
    result.push_back(name);
  }
  cache_.clear();
  return result;
}

template <typename T>
std::vector<std::string> Ruler<T>::disable(const std::string& name,
                                           bool ignoreInvalid) {
  return disable(std::vector<std::string>{name}, ignoreInvalid);
}

template <typename T>
std::vector<T> Ruler<T>::getRules(const std::string& chainName) const {
  if (cache_.empty()) {
    compile();
  }

  auto it = cache_.find(chainName);
  if (it == cache_.end()) {
    // return empty vector if chain not present
    return {};
  }
  return it->second;
}

}  // namespace aethermark
