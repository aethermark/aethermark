// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "aethermark/parser_block.hpp"
#include "aethermark/parser_core.hpp"
// #include "aethermark/parser_inline.hpp"
// #include "aethermark/renderer.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

/// @brief Options configuration object for Aethermark.
struct Options {
  /// @brief HTML.
  bool html = false;

  /// @brief XHTML out.
  bool xhtml_out = false;

  /// @brief Breaks.
  bool breaks = false;

  /// @brief Language prefix for CSS.
  std::string lang_prefix = "language-";

  /// @brief Linkify.
  bool linkify = false;

  /// @brief Typographer.
  bool typographer = false;

  /// @brief List of quotes.
  std::vector<std::string> quotes = {"“", "”", "‘", "’"};

  /// @brief Highlight function.
  std::function<std::string(const std::string&, const std::string&,
                            const std::string&)>
      highlight = nullptr;

  /// @brief Maximum nesting.
  int max_nesting = 20;
};

/// @brief Core parser configuration.
struct CoreConfig {
  std::vector<std::string> rules;  ///< Rules array.
};

/// @brief Block parser configuration.
struct BlockConfig {
  std::vector<std::string> rules;  ///< Rules array.
};

/// @brief Inline parser configuration.
struct InlineConfig {
  std::vector<std::string> rules_1;  ///< First rules array.
  std::vector<std::string> rules_2;  ///< Second rules array.
};

/// @brief Rules configuration.
struct ComponentConfig {
  CoreConfig core_config;      ///< Core parser configuration.
  BlockConfig block_config;    ///< Block parser configuration.
  InlineConfig inline_config;  ///< Inline parser configuration.
};

/// @brief Configuration preset object for Aethermark.
struct Preset {
  Options options;             ///< Options configuration.
  ComponentConfig components;  ///< Component configuration.
};

/// @brief Main entry for Aethermark.
class Aethermark {
 public:
  /// @brief Main instance of inline parser.
  // ParserInline inline_parser;

  /// @brief Main instance of block parser.
  ParserBlock block_parser;

  /// @brief Main instance of core parser.
  ParserCore core_parser;

  /// @brief Main instance of renderer.
  // Renderer renderer;

  /// @brief Confgurations of processor.
  Options options;

  /// @brief The configuration presets.
  static const std::unordered_map<std::string, Preset> presets;

  // TODO(MukulWaval): complete documentation.
  Aethermark();
  explicit Aethermark(const Options& opts);
  explicit Aethermark(std::string preset_name,
                      std::optional<Options> options = std::nullopt);

  // configure options on existing instance
  Aethermark& Set(const Options& opts);

  // presets: "default", "zero", "commonmark"
  Aethermark& Configure(const std::string& preset_name);
  Aethermark& Configure(const Preset& preset);

  // enable/disable rules: dispatch into block/core/inline
  Aethermark& Enable(const std::vector<std::string>& list,
                     bool ignore_invalid = false);
  Aethermark& Disable(const std::vector<std::string>& list,
                      bool ignore_invalid = false);

  // parse
  std::deque<Token> Parse(const std::string& src, std::any env);
  std::deque<Token> ParseInline(const std::string& src, std::any env);

  // render
  std::string Render(const std::string& src, std::any env = nullptr);
  std::string RenderInline(const std::string& src, std::any env = nullptr);

  // link handling (placeholder for now)
  bool ValidateLink(const std::string& url) const;
  std::string NormalizeLink(const std::string& url) const;
  std::string NormalizeLinkText(const std::string& url) const;

  // plugins
  Aethermark& Use(std::function<void(Aethermark&)> plugin);
};

}  // namespace aethermark
