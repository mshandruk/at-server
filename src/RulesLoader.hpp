#pragma once

#include <istream>
#include <string>
#include <vector>

struct AtRule {
    std::string commandPattern;
    std::string response;
};

using AtRules = std::vector<AtRule>;

AtRules loadRules(std::istream& in);
