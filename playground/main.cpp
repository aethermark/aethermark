#include <any>
#include <string>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/parser_block.hpp"
#include "aethermark/token.hpp"

int main() {
  std::string src = "> Blockquotes can also be nested...";
  aethermark::Aethermark md;
  std::any env;

  std::vector<aethermark::Token> tokens;

  // Option 1: use parse (recommended)
  md.blockParser.parse(src, md, env, tokens);

  // Option 2: manually create state (less common)
  /*
  aethermark::ParserBlock parser;
  aethermark::StateBlock state(src, md, env, tokens);
  parser.tokenize(state, 0, state.lineMax);
  */

  return 0;
}
