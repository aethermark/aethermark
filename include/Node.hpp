#pragma once
#include <memory>
#include <pybind11/pybind11.h>
#include <string>
#include "NodeWalker.hpp"

class Node : public std::enable_shared_from_this<Node>
{
public:
    /* ────────── Constructors ────────── */

    Node(const std::string &type, const std::vector<std::vector<int>> &sourcepos = {});

    /* ────────── Methods ────────── */

    void appendChild(std::shared_ptr<Node> child);
    void prependChild(std::shared_ptr<Node> child);
    void unlink();
    void insertAfter(std::shared_ptr<Node> sibling);
    void insertBefore(std::shared_ptr<Node> sibling);
    bool isContainer() const;
    std::unique_ptr<NodeWalker> walker();

    /* ────────── Properties ────────── */

    std::string getType() const;
    void setType(const std::string &s);

    std::vector<std::vector<int>> getSourcepos() const;
    void setSourcepos(const std::vector<std::vector<int>> &s);

    std::string getLiteral() const;
    void setLiteral(const std::string &s);

    std::string getDestination() const;
    void setDestination(const std::string &s);

    std::string getTitle() const;
    void setTitle(const std::string &s);

    std::string getInfo() const;
    void setInfo(const std::string &s);

    int getLevel() const;
    void setLevel(int s);

    /* ────────── Readonly Properties ────────── */

    std::shared_ptr<Node> getFirstChild() const;

    std::shared_ptr<Node> getLastChild() const;

    std::shared_ptr<Node> getNext() const;

    std::shared_ptr<Node> getPrev() const;

    std::shared_ptr<Node> getParent() const;

    /* ────────── Binding Methods ────────── */

    static void bind(pybind11::module &m);

private:
    std::string _type;
    std::vector<std::vector<int>> _sourcepos;
    std::string _literal;
    std::string _destination;
    std::string _title;
    std::string _info;
    int _level;

    std::shared_ptr<Node> _parent;
    std::shared_ptr<Node> _firstChild;
    std::shared_ptr<Node> _lastChild;
    std::shared_ptr<Node> _prev;
    std::shared_ptr<Node> _next;
};
