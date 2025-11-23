// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace aethermark {

/// @brief Configuration options of a rule.
struct RuleOptions {
  /// @brief Array with names of "alternate" chains.
  std::vector<std::string> alt;
};

/// @brief A rule item for Ruler class.
/// @tparam T The function type of the rule.
template <typename T>
struct Rule {
  /// @brief Name of the rule.
  std::string name;

  /// @brief The rule function.
  T fn;

  /// @brief Alternate chains this rule belongs to.
  std::vector<std::string> alt;

  /// @brief Whether the rule is enabled.
  bool enabled = true;
};

/// @brief A rule manager.
/// @tparam T The function type of the rule.
template <typename T>
class Ruler {
 public:
  Ruler() = default;

  /// @brief Replace rule by name with new function & options.
  /// @param name The name of the rule to replace.
  /// @param fn The new function for the rule.
  /// @param options The options for the rule.
  /// @exception `std::runtime_error` if rule name not found.
  void at(const std::string& name, T fn, const RuleOptions& options = {});

  /// @brief Add new rule to chain before one with given name.
  /// @param beforeName Name of the rule to insert before.
  /// @param ruleName Name of the new rule.
  /// @param fn The function of the new rule.
  /// @param options The options for the new rule.
  /// @exception `std::runtime_error` if `beforeName` not found.
  void before(const std::string& beforeName, const std::string& ruleName, T fn,
              const RuleOptions& options = {});

  /// @brief Add new rule to chain after one with given name.
  /// @param afterName Name of the rule to insert after.
  /// @param ruleName Name of the new rule.
  /// @param fn The function of the new rule.
  /// @param options The options for the new rule.
  /// @exception `std::runtime_error` if `afterName` not found.
  void after(const std::string& afterName, const std::string& ruleName, T fn,
             const RuleOptions& options = {});

  /// @brief Push new rule to the end of chain.
  /// @param ruleName Name of the new rule.
  /// @param fn The function of the new rule.
  /// @param options The options for the new rule.
  void push(const std::string& ruleName, T fn, const RuleOptions& options = {});

  /// @brief Enable rules with given names.
  /// @param list List of rule names to enable.
  /// @param ignoreInvalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignoreInvalid` is `false`.
  std::vector<std::string> enable(const std::vector<std::string>& list,
                                  bool ignoreInvalid = false);

  /// @brief Enable rule with given name.
  /// @param name Name of the rule to enable.
  /// @param ignoreInvalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignoreInvalid` is `false`.
  std::vector<std::string> enable(const std::string& name,
                                  bool ignoreInvalid = false);

  /// @brief Enable rules with given names, and disable everything else.
  /// @param list List of rule names to enable.
  /// @param ignoreInvalid set `true` to ignore errors when rule not found.
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignoreInvalid` is `false`.
  void enableOnly(const std::vector<std::string>& list,
                  bool ignoreInvalid = false);

  /// @brief Disable rules with given names.
  /// @param list List of rule names to disable.
  /// @param ignoreInvalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignoreInvalid` is `false`.
  std::vector<std::string> disable(const std::vector<std::string>& list,
                                   bool ignoreInvalid = false);

  /// @brief Disable rule with the given name.
  /// @param name Name of the rule to disable.
  /// @param ignoreInvalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignoreInvalid` is `false`.
  std::vector<std::string> disable(const std::string& name,
                                   bool ignoreInvalid = false);

  /// @brief Get list of enabled rules for given chain.
  /// @param chainName Name of the chain.
  /// @return Array of rule functions.
  std::vector<T> getRules(const std::string& chainName) const;

  /// @brief Find rule by name.
  /// @param name Name of the rule.
  /// @return Pointer to the rule if found, `nullptr` otherwise.
  Rule<T>* findRule(const std::string& name);

  /// @brief Find rule by name (const version).
  /// @param name Name of the rule.
  /// @return Pointer to the rule if found, `nullptr` otherwise.
  const Rule<T>* findRule(const std::string& name) const;

 private:
  std::vector<Rule<T>> rules_;
  mutable std::unordered_map<std::string, std::vector<T>> cache_;

  void compile() const;
  void touch() const { cache_.clear(); }
};

}  // namespace aethermark

#include "ruler.tpp"
