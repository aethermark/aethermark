// NOLINT(legal/copyright)

#include <aethermark/ruler.hpp>  // NOLINT(build/include_order)
#include <algorithm>             // NOLINT(build/include_order)
#include <functional>            // NOLINT(build/include_order)
#include <iostream>              // NOLINT(build/include_order)
#include <string>                // NOLINT(build/include_order)

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

int main() {
  using aethermark::RuleOptions;
  using aethermark::Ruler;

  Ruler<Fn> r;

  // Add some rules to play with
  r.push("trim", trim);
  r.push("upper", toUpper);
  r.push("dashes", replaceDashes, RuleOptions{{"alt_dashes"}});

  std::string input = "   hello-world  ";
  std::string out = input;

  std::cout << "Input: '" << input << "'\n";

  // Apply default chain
  for (auto& fn : r.getRules("")) {
    out = fn(out);
  }

  std::cout << "Default chain output: '" << out << "'\n";

  // Apply alternate chain
  std::string altOut = input;
  for (auto& fn : r.getRules("alt_dashes")) {
    altOut = fn(altOut);
  }

  std::cout << "Alt chain output: '" << altOut << "'\n";

  // Touch replace — try replacing a rule
  r.at("trim", [](std::string s) { return "[TRIMMED]" + trim(s); });

  std::string replaced = input;
  for (auto& fn : r.getRules("")) {
    replaced = fn(replaced);
  }

  std::cout << "After replacing 'trim': '" << replaced << "'\n";

  return 0;
}
