// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "aethermark/parser_block.hpp"
#include "aethermark/parser_core.hpp"
// #include "aethermark/parser_inline.hpp"
// #include "aethermark/renderer.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

struct Options {
  bool html = false;
  bool xhtmlOut = false;
  bool breaks = false;
  std::string langPrefix = "language-";
  bool linkify = false;
  bool typographer = false;

  std::vector<std::string> quotes = {"“", "”", "‘", "’"};

  std::function<std::string(const std::string&, const std::string&,
                            const std::string&)>
      highlight = nullptr;

  int maxNesting = 20;
};

using ParserCore = Core;

class Aethermark {
 public:
  // core components
  // ParserInline inlineParser;
  ParserBlock blockParser;
  ParserCore coreParser;
  // Renderer renderer;

  Options options;

  Aethermark();
  explicit Aethermark(const Options& opts);

  // configure options on existing instance
  Aethermark& set(const Options& opts);

  // presets: "default", "zero", "commonmark"
  Aethermark& configure(const std::string& preset);

  // enable/disable rules: dispatch into block/core/inline
  Aethermark& enable(const std::vector<std::string>& list,
                     bool ignoreInvalid = false);
  Aethermark& disable(const std::vector<std::string>& list,
                      bool ignoreInvalid = false);

  // parse
  std::vector<Token> parse(const std::string& src, std::any env);
  std::vector<Token> parseInline(const std::string& src, std::any env);

  // render
  std::string render(const std::string& src, std::any env = nullptr);
  std::string renderInline(const std::string& src, std::any env = nullptr);

  // link handling (placeholder for now)
  bool validateLink(const std::string& url) const;
  std::string normalizeLink(const std::string& url) const;
  std::string normalizeLinkText(const std::string& url) const;

  // plugins
  Aethermark& use(std::function<void(Aethermark&)> plugin);
};

}  // namespace aethermark
