// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/aethermark.hpp"

#include <deque>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// #include "aethermark/parser_block.hpp"
#include "aethermark/parser_core.hpp"
// #include "aethermark/parser_inline.hpp"
// #include "aethermark/renderer.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

// TODO(MukulWaval): Add default move, copy constructors and destructors for all
// clasess
// TODO(MukulWaval): Add documentation for all classes
// TODO(MukulWaval): Optimize the use of std::vector
// TODO(MukulWaval): Remove all unnecessary comments from all files
// TODO(MukulWaval): Implement missing Aethermark class

const std::unordered_map<std::string, Preset> Aethermark::presets = {
    {"default",
     Preset{.options = Options{.html = false,
                               .xhtml_out = false,
                               .breaks = false,
                               .lang_prefix = "language-",
                               .linkify = false,
                               .typographer = false,
                               .quotes = {"\u201c", "\u201d", "\u2018",
                                          "\u2019"},  // “”‘’
                               .highlight = nullptr,
                               .max_nesting = 100},
            .components =
                ComponentConfig{.core_config = CoreConfig{.rules = {}},
                                .block_config = BlockConfig{.rules = {}},
                                .inline_config = InlineConfig{.rules_1 = {},
                                                              .rules_2 = {}}}}},
    {"zero", Preset{.options = Options{.html = false,
                                       .xhtml_out = false,
                                       .breaks = false,
                                       .lang_prefix = "language-",
                                       .linkify = false,
                                       .typographer = false,
                                       .quotes = {"\u201c", "\u201d", "\u2018",
                                                  "\u2019"},  // “”‘’
                                       .highlight = nullptr,
                                       .max_nesting = 20},
                    .components =
                        ComponentConfig{
                            .core_config =
                                CoreConfig{.rules = {"normalize", "block",
                                                     "inline", "text_join"}},

                            .block_config = BlockConfig{.rules = {"paragraph"}},

                            .inline_config =
                                InlineConfig{
                                    .rules_1 = {"text"},
                                    .rules_2 = {"balance_pairs",
                                                "fragments_join"}}}}},
    {"commonmark",
     Preset{
         .options = Options{.html = true,
                            .xhtml_out = true,
                            .breaks = false,
                            .lang_prefix = "language-",
                            .linkify = false,
                            .typographer = false,
                            .quotes = {"\u201c", "\u201d", "\u2018",
                                       "\u2019"},  // “”‘’
                            .highlight = nullptr,
                            .max_nesting = 20},

         .components = ComponentConfig{
             .core_config = CoreConfig{.rules = {"normalize", "block", "inline",
                                                 "text_join"}},

             .block_config =
                 BlockConfig{.rules = {"blockquote", "code", "fence", "heading",
                                       "hr", "html_block", "lheading", "list",
                                       "reference", "paragraph"}},

             .inline_config =
                 InlineConfig{.rules_1 = {"autolink", "backticks", "emphasis",
                                          "entity", "escape", "html_inline",
                                          "image", "link", "newline", "text"},
                              .rules_2 = {"balance_pairs", "emphasis",
                                          "fragments_join"}}}}}};

Aethermark::Aethermark()
    : /*inlineParser(),*/ block_parser(), core_parser() /* , renderer(),*/ {
  Configure("default");
}

Aethermark::Aethermark(const Options& opts)
    : /*inlineParser(),*/ block_parser(), core_parser() /*, renderer(), */ {
  Configure("default");
  Set(opts);
}

Aethermark::Aethermark(std::string preset_name, std::optional<Options> options)
    : /*inlineParser(),*/ block_parser(), core_parser() /*, renderer(), */ {
  Configure(preset_name);

  if (options.has_value()) {
    Set(*options);
  }
}

Aethermark& Aethermark::Set(const Options& opts) {
  options = opts;
  return *this;
}

Aethermark& Aethermark::Configure(const std::string& preset_name) {
  auto it = Aethermark::presets.find(preset_name);
  if (it == Aethermark::presets.end()) {
    throw std::runtime_error("Unknown preset: " + preset_name);
  }
  Preset preset = it->second;

  Configure(preset);

  return *this;
}

Aethermark& Aethermark::Configure(const Preset& preset) {
  Set(preset.options);

  core_parser.ruler.EnableOnly(preset.components.core_config.rules);
  block_parser.ruler.EnableOnly(preset.components.block_config.rules);
  // inline_parser.ruler.EnableOnly(preset.components.inline_config.rules_1);
  // inline_parser.rule2.EnableOnly(preset.components.inline_config.rules_2);

  return *this;
}

// Aethermark& Aethermark::Enable(const std::vector<std::string>& list,
//                                bool ignoreInvalid) {
//   std::vector<std::string> enabled;
//
//   for (auto* chain :
//        {&core_parser.ruler, &block_parser.ruler /*, &inline_parser.ruler*/})
//        {
//     auto r = chain->enable(list, true);
//     enabled.insert(enabled.end(), r.begin(), r.end());
//   }
//
//   // inline ruler2
//   // if (inline_parser.ruler2.has_value()) {
//   //   auto r2 = inline_parser.ruler2->enable(list, true);
//   //   enabled.insert(enabled.end(), r2.begin(), r2.end());
//   // }
//
//   for (const auto& name : list) {
//     if (std::find(enabled.begin(), enabled.end(), name) == enabled.end()) {
//       if (!ignoreInvalid) {
//         throw std::runtime_error("Unknown rule: " + name);
//       }
//     }
//   }
//   return *this;
// }
//
// Aethermark& Aethermark::disable(const std::vector<std::string>& list,
//                                 bool ignoreInvalid) {
//   std::vector<std::string> disabled;
//
//   for (auto* chain :
//        {&coreParser.ruler, &blockParser.ruler, &inlineParser.ruler}) {
//     auto r = chain->disable(list, true);
//     disabled.insert(disabled.end(), r.begin(), r.end());
//   }
//
//   if (inlineParser.ruler2.has_value()) {
//     auto r2 = inlineParser.ruler2->disable(list, true);
//     disabled.insert(disabled.end(), r2.begin(), r2.end());
//   }
//
//   for (const auto& name : list) {
//     if (std::find(disabled.begin(), disabled.end(), name) == disabled.end())
//     {
//       if (!ignoreInvalid) {
//         throw std::runtime_error("Unknown rule: " + name);
//       }
//     }
//   }
//   return *this;
// }

std::deque<Token> Aethermark::Parse(const std::string& src, std::any env) {
  StateCore state(src, *this, env);
  core_parser.Process(state);
  return state.tokens;
}

std::deque<Token> Aethermark::ParseInline(const std::string& src,
                                          std::any env) {
  StateCore state(src, *this, env);
  state.inline_mode = true;
  core_parser.Process(state);
  return state.tokens;
}

// std::string Aethermark::render(const std::string& src, std::any env) {
//   auto tokens = parse(src, env);
//   return renderer.render(tokens, options, env);
// }

// std::string Aethermark::renderInline(const std::string& src, std::any env) {
//   auto tokens = parseInline(src, env);
//   return renderer.render(tokens, options, env);
// }

// bool Aethermark::validateLink(const std::string& url) const {
//   // Basic match to Markdown-It’s behavior
//   std::string lower;
//   lower.reserve(url.size());
//   for (char c : url) lower.push_back(std::tolower(c));

//   if (lower.starts_with("javascript:") || lower.starts_with("vbscript:") ||
//       lower.starts_with("file:")) {
//     return false;
//   }

//   if (lower.starts_with("data:")) {
//     if (lower.starts_with("data:image/gif") ||
//         lower.starts_with("data:image/png") ||
//         lower.starts_with("data:image/jpeg") ||
//         lower.starts_with("data:image/webp")) {
//       return true;
//     }
//     return false;
//   }
//   return true;
// }

std::string Aethermark::NormalizeLink(const std::string& url) const {
  // TODO(MukulWaval): add proper mdurl + punycode equivalent
  return url;
}

std::string Aethermark::NormalizeLinkText(const std::string& url) const {
  // TODO(MukulWaval): add proper unicode/punycode normalization
  return url;
}

Aethermark& Aethermark::Use(std::function<void(Aethermark&)> plugin) {
  plugin(*this);
  return *this;
}

}  // namespace aethermark
