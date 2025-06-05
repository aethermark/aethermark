#include "token.hpp"

Token::Token(std::string type, std::string tag, Nesting nesting)
    : type(std::move(type)), tag(std::move(tag)), nesting(nesting),
      level(0), block(false), hidden(false) {}

int Token::attrIndex(const std::string &name) const
{
    for (size_t i = 0; i < attrs.size(); ++i)
    {
        if (attrs[i].first == name)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Token::attrPush(const std::pair<std::string, std::string> &attrData)
{
    attrs.push_back(attrData);
}

void Token::attrSet(const std::string &name, const std::string &value)
{
    int idx = attrIndex(name);
    if (idx < 0)
    {
        attrPush({name, value});
    }
    else
    {
        attrs[idx] = {name, value};
    }
}

std::optional<std::string> Token::attrGet(const std::string &name) const
{
    int idx = attrIndex(name);
    if (idx >= 0)
    {
        return attrs[idx].second;
    }
    return std::nullopt;
}

void Token::attrJoin(const std::string &name, const std::string &value)
{
    int idx = attrIndex(name);
    if (idx < 0)
    {
        attrPush({name, value});
    }
    else
    {
        attrs[idx].second += " " + value;
    }
}
