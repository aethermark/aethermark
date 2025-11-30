// NOLINT(legal/copyright)

#include <aethermark/aethermark.hpp>   // NOLINT(build/include_order)
#include <aethermark/parser_core.hpp>  // NOLINT(build/include_order)
#include <aethermark/ruler.hpp>        // NOLINT(build/include_order)
#include <algorithm>                   // NOLINT(build/include_order)
#include <functional>                  // NOLINT(build/include_order)
#include <iostream>                    // NOLINT(build/include_order)
#include <string>                      // NOLINT(build/include_order)

using Fn = std::function<std::string(std::string)>;

std::string trim(std::string s) {
  while (!s.empty() && isspace(s.front())) s.erase(s.begin());
  while (!s.empty() && isspace(s.back())) s.pop_back();
  return s;
}

std::string toUpper(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
}

std::string replaceDashes(std::string s) {
  std::replace(s.begin(), s.end(), '-', ' ');
  return s;
}

// FIXME: This is just a playground for testing Ruler functionalities.
int main() {
  using aethermark::RuleOptions;
  using aethermark::Ruler;

  aethermark::Core coreParser = aethermark::Core();

  std::string src = "This is   a - sample\n\ncore parser   input.";
  aethermark::Aethermark md = aethermark::Aethermark();
  std::any env = std::any(1);

  std::cout << "Starting core parser processing...\n";

  aethermark::StateCore state = aethermark::StateCore(src, md, env);
  coreParser.process(state);
}
