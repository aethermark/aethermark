#include "ruler.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

template <typename T>
int Ruler<T>::findRuleIndex(const std::string &name) const
{
    for (size_t i = 0; i < rules.size(); ++i)
    {
        if (rules[i].name == name)
            return static_cast<int>(i);
    }
    return -1;
}

template <typename T>
void Ruler<T>::compileCache()
{
    cache.clear();
    std::vector<std::string> chains{""};

    for (const auto &rule : rules)
    {
        if (!rule.enabled)
            continue;
        for (const auto &alt : rule.alt)
        {
            if (std::find(chains.begin(), chains.end(), alt) == chains.end())
            {
                chains.push_back(alt);
            }
        }
    }

    for (const auto &chain : chains)
    {
        std::vector<T> chainRules;
        for (const auto &rule : rules)
        {
            if (!rule.enabled)
                continue;
            if (!chain.empty() && std::find(rule.alt.begin(), rule.alt.end(), chain) == rule.alt.end())
                continue;
            chainRules.push_back(rule.fn);
        }
        cache[chain] = std::move(chainRules);
    }

    cacheDirty = false;
}

template <typename T>
void Ruler<T>::at(const std::string &name, T fn, RuleOptions options)
{
    int idx = findRuleIndex(name);
    if (idx < 0)
        throw std::runtime_error("Rule not found: " + name);
    rules[idx].fn = fn;
    rules[idx].alt = options.alt;
    cacheDirty = true;
}

template <typename T>
void Ruler<T>::before(const std::string &beforeName, const std::string &ruleName, T fn, RuleOptions options)
{
    int idx = findRuleIndex(beforeName);
    if (idx < 0)
        throw std::runtime_error("Rule not found: " + beforeName);
    rules.insert(rules.begin() + idx, Rule<T>{ruleName, true, fn, options.alt});
    cacheDirty = true;
}

template <typename T>
void Ruler<T>::after(const std::string &afterName, const std::string &ruleName, T fn, RuleOptions options)
{
    int idx = findRuleIndex(afterName);
    if (idx < 0)
        throw std::runtime_error("Rule not found: " + afterName);
    rules.insert(rules.begin() + idx + 1, Rule<T>{ruleName, true, fn, options.alt});
    cacheDirty = true;
}

template <typename T>
void Ruler<T>::push(const std::string &ruleName, T fn, RuleOptions options)
{
    rules.push_back(Rule<T>{ruleName, true, fn, options.alt});
    cacheDirty = true;
}

template <typename T>
std::vector<std::string> Ruler<T>::enable(const std::vector<std::string> &list, bool ignoreInvalid)
{
    std::vector<std::string> result;
    for (const auto &name : list)
    {
        int idx = findRuleIndex(name);
        if (idx < 0)
        {
            if (!ignoreInvalid)
                throw std::runtime_error("Invalid rule name: " + name);
        }
        else
        {
            rules[idx].enabled = true;
            result.push_back(name);
        }
    }
    cacheDirty = true;
    return result;
}

template <typename T>
std::vector<std::string> Ruler<T>::disable(const std::vector<std::string> &list, bool ignoreInvalid)
{
    std::vector<std::string> result;
    for (const auto &name : list)
    {
        int idx = findRuleIndex(name);
        if (idx < 0)
        {
            if (!ignoreInvalid)
                throw std::runtime_error("Invalid rule name: " + name);
        }
        else
        {
            rules[idx].enabled = false;
            result.push_back(name);
        }
    }
    cacheDirty = true;
    return result;
}

template <typename T>
void Ruler<T>::enableOnly(const std::vector<std::string> &list, bool ignoreInvalid)
{
    for (auto &rule : rules)
        rule.enabled = false;
    enable(list, ignoreInvalid);
}

template <typename T>
std::vector<T> Ruler<T>::getRules(const std::string &chainName)
{
    if (cacheDirty)
        compileCache();
    auto it = cache.find(chainName);
    return it != cache.end() ? it->second : std::vector<T>{};
}

// Explicit template instantiation (for function pointer types or lambdas)
template class Ruler<std::function<void(py::object)>>;
