#include "RulesLoader.hpp"

#include <cstddef>
#include <iostream>
#include <istream>
#include <string>

AtRules loadRules(std::istream& in) {
    AtRules rules;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.rfind('#', 0) == 0) {
            continue;
        }

        const std::size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        rules.emplace_back(AtRule{
            line.substr(0, pos),
            line.substr(pos + 1),
        });
    }

    return rules;
}
