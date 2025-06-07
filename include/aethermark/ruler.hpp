#pragma once
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "types/rule.hpp"
#include "types/rule_options.hpp"

namespace aethermark
{
    template <typename T>
    class Ruler
    {
    public:
        Ruler() = default;

        void at(const std::string &name, T fn, types::RuleOptions options = {});
        void before(const std::string &beforeName, const std::string &ruleName, T fn, types::RuleOptions options = {});
        void after(const std::string &afterName, const std::string &ruleName, T fn, types::RuleOptions options = {});
        void push(const std::string &ruleName, T fn, types::RuleOptions options = {});
        std::vector<std::string> enable(const std::vector<std::string> &list, bool ignoreInvalid = false);
        std::vector<std::string> disable(const std::vector<std::string> &list, bool ignoreInvalid = false);
        void enableOnly(const std::vector<std::string> &list, bool ignoreInvalid = false);
        std::vector<T> getRules(const std::string &chainName);

    private:
        int findRuleIndex(const std::string &name) const;
        void compileCache();

        std::vector<types::Rule<T>> rules;
        std::unordered_map<std::string, std::vector<T>> cache;
        bool cacheDirty = true;
    };
} // namespace aethermark
