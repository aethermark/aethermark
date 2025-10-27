// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace aethermark {

/// @brief Options for a rule.
struct RuleOptions {
  /// @brief Alternative chain names for this rule.
  std::vector<std::string> alt;
};

template <typename T>
/// @brief Internal structure to represent a rule item.
struct RuleItem {
  std::string name;
  bool enabled;
  T fn;
  std::vector<std::string> alt;
};

/// @brief Helper class to manage rules.
///
/// Responsibilities include:
/// - Keeping rules in a defined order
/// - Assigning a name to each rule
/// - Enabling or disabling rules
/// - Adding or replacing rules
/// - Assigning rules to additional named chains (within the same context)
/// - Caching lists of active rules
/// @tparam T Type parameter representing the rule type.
template <typename T>
class Ruler {
 public:
  /// @brief Constructor for a new Ruler object.
  Ruler() = default;

  /// @brief Replace rule by name with new function & options.
  /// @param name Rule name to replace.
  /// @param fn New rule function.
  /// @param options New rule options (not mandatory).
  /// @throw `runtime_error` if rule name not found.
  void at(const std::string& name, const T& fn,
          const RuleOptions& options = {});

  /// @brief Add new rule to chain before one with given name.
  /// @param beforeName New rule will be added before this one.
  /// @param ruleName Name of added rule.
  /// @param fn Rule function.
  /// @param options Rule options (not mandatory).
  /// @throw `runtime_error` if rule name not found.
  void before(const std::string& beforeName, const std::string& ruleName,
              const T& fn, const RuleOptions& options = {});

  /// @brief Add new rule to chain after one with given name.
  /// @param afterName New rule will be added after this one.
  /// @param ruleName Name of added rule.
  /// @param fn Rule function.
  /// @param options Rule options (not mandatory).
  /// @throw `runtime_error` if rule name not found.
  void after(const std::string& afterName, const std::string& ruleName,
             const T& fn, const RuleOptions& options = {});

  /// @brief Push new rule to the end of chain.
  /// @param ruleName Name of added rule.
  /// @param fn Rule function.
  /// @param options Rule options (not mandatory).
  void push(const std::string& ruleName, const T& fn,
            const RuleOptions& options = {});

  /// @brief Enable rules with given names.
  /// @param list List of rule names to enable.
  /// @param ignoreInvalid Set `true` to ignore errors when rule not found.
  /// @return Returns list of found rule names.
  /// @throw `runtime_error` if invalid rule name found and ignoreInvalid is
  /// false.
  std::vector<std::string> enable(const std::vector<std::string>& list,
                                  bool ignoreInvalid = false);

  /// @brief Enable rules with given names.
  /// @param name Rule name to enable.
  /// @param ignoreInvalid Set `true` to ignore errors when rule not found.
  /// @return Returns list of found rule names.
  /// @throw `runtime_error` if invalid rule name found and ignoreInvalid is
  /// false.
  std::vector<std::string> enable(const std::string& name,
                                  bool ignoreInvalid = false);

  /// @brief Enable rules with given names, and disable everything else.
  /// @param list List of rule names to enable (whitelist).
  /// @param ignoreInvalid Set `true` to ignore errors when rule not found.
  void enableOnly(const std::vector<std::string>& list,
                  bool ignoreInvalid = false);

  /// @brief Enable rules with given names, and disable everything else.
  /// @param name Rule name to enable (whitelist).
  /// @param ignoreInvalid Set `true` to ignore errors when rule not found.
  void enableOnly(const std::string& name, bool ignoreInvalid = false);

  /// @brief Disable rules with given names.
  /// @param list List of rule names to disable.
  /// @param ignoreInvalid Set `true` to ignore errors when rule not found.
  /// @return Returns list of found rule names.
  /// @throw `runtime_error` if invalid rule name found and ignoreInvalid is
  /// false.
  std::vector<std::string> disable(const std::vector<std::string>& list,
                                   bool ignoreInvalid = false);

  /// @brief Disable rules with given names.
  /// @param name Rule name to disable.
  /// @param ignoreInvalid Set `true` to ignore errors when rule not found.
  /// @return Returns list of found rule names.
  /// @throw `runtime_error` if invalid rule name found and ignoreInvalid is
  /// false.
  std::vector<std::string> disable(const std::string& name,
                                   bool ignoreInvalid = false);

  /// @brief Get active functions for given chain name.
  /// @param chainName Name of the chain.
  /// @return Array of active functions (rules) for given chain name.
  std::vector<T> getRules(const std::string& chainName) const;

  std::vector<RuleItem<T>> getRules() const { return rules_; }
  std::unordered_map<std::string, std::vector<T>> getCache() const {
    return cache_;
  }

 private:
  /// @brief List of all rules.
  std::vector<RuleItem<T>> rules_;

  /// @brief Cache of compiled rule lists for different chains.
  mutable std::unordered_map<std::string, std::vector<T>> cache_;

  /// @brief Find index of rule by name.
  /// @param name Name of the rule.
  /// @return
  int find(const std::string& name) const;

  /// @brief Build rules lookup cache.
  void compile() const;
};
}  // namespace aethermark

#include "ruler.tpp"
