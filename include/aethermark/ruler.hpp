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
  void At(const std::string& name, T fn, const RuleOptions& options = {});

  /// @brief Add new rule to chain before one with given name.
  /// @param before_name Name of the rule to insert before.
  /// @param rule_name Name of the new rule.
  /// @param fn The function of the new rule.
  /// @param options The options for the new rule.
  /// @exception `std::runtime_error` if `beforeName` not found.
  void Before(const std::string& before_name, const std::string& rule_name,
              T fn, const RuleOptions& options = {});

  /// @brief Add new rule to chain after one with given name.
  /// @param after_name Name of the rule to insert after.
  /// @param rule_name Name of the new rule.
  /// @param fn The function of the new rule.
  /// @param options The options for the new rule.
  /// @exception `std::runtime_error` if `afterName` not found.
  void After(const std::string& after_name, const std::string& rule_name, T fn,
             const RuleOptions& options = {});

  /// @brief Push new rule to the end of chain.
  /// @param rule_name Name of the new rule.
  /// @param fn The function of the new rule.
  /// @param options The options for the new rule.
  void Push(const std::string& rule_name, T fn,
            const RuleOptions& options = {});

  /// @brief Enable rules with given names.
  /// @param list List of rule names to enable.
  /// @param ignore_invalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignore_invalid` is `false`.
  std::vector<std::string> Enable(const std::vector<std::string>& list,
                                  bool ignore_invalid = false);

  /// @brief Enable rule with given name.
  /// @param name Name of the rule to enable.
  /// @param ignore_invalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignore_invalid` is `false`.
  std::vector<std::string> Enable(const std::string& name,
                                  bool ignore_invalid = false);

  /// @brief Enable rules with given names, and disable everything else.
  /// @param list List of rule names to enable.
  /// @param ignore_invalid set `true` to ignore errors when rule not found.
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignore_invalid` is `false`.
  void EnableOnly(const std::vector<std::string>& list,
                  bool ignore_invalid = false);

  /// @brief Disable rules with given names.
  /// @param list List of rule names to disable.
  /// @param ignore_invalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignore_invalid` is `false`.
  std::vector<std::string> Disable(const std::vector<std::string>& list,
                                   bool ignore_invalid = false);

  /// @brief Disable rule with the given name.
  /// @param name Name of the rule to disable.
  /// @param ignore_invalid set `true` to ignore errors when rule not found.
  /// @return List of found rule names (if no exception happened).
  /// @exception `std::runtime_error` if any rule name not found and
  /// `ignore_invalid` is `false`.
  std::vector<std::string> Disable(const std::string& name,
                                   bool ignore_invalid = false);

  /// @brief Get list of enabled rules for given chain.
  /// @param chain_name Name of the chain.
  /// @return Array of rule functions.
  std::vector<T> GetRules(const std::string& chain_name) const;

  /// @brief Find rule by name.
  /// @param name Name of the rule.
  /// @return Pointer to the rule if found, `nullptr` otherwise.
  Rule<T>* FindRule(const std::string& name);

  /// @brief Find rule by name (const version).
  /// @param name Name of the rule.
  /// @return Pointer to the rule if found, `nullptr` otherwise.
  const Rule<T>* FindRule(const std::string& name) const;

 private:
  std::vector<Rule<T>> rules_;
  mutable std::unordered_map<std::string, std::vector<T>> cache_;

  void Compile() const;
  void Touch() const { cache_.clear(); }
};

}  // namespace aethermark

#include "ruler.tpp"
