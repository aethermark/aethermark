#pragma once

#include <string>
#include <vector>
#include <optional>
#include <utility>
#include "helpers/nesting.hpp"

class Token
{
public:
    Token(std::string type, std::string tag, Nesting nesting);

    int attrIndex(const std::string &name) const;
    void attrPush(const std::pair<std::string, std::string> &attrData);
    void attrSet(const std::string &name, const std::string &value);
    std::optional<std::string> attrGet(const std::string &name) const;
    void attrJoin(const std::string &name, const std::string &value);

    std::string type;
    std::string tag;
    std::vector<std::pair<std::string, std::string>> attrs;
    std::optional<std::pair<int, int>> map;
    Nesting nesting;
    int level;
    std::optional<std::vector<Token>> children;
    std::string content;
    std::string markup;
    std::string info;
    std::optional<std::string> meta;
    bool block;
    bool hidden;
};
