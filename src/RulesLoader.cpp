#include "RulesLoader.hpp"

#include <cstddef>
#include <iostream>
#include <istream>
#include <string>

std::string decodeEscapeSeq(const std::string& src) {
    std::string result;
    result.reserve(src.size());

    for (std::size_t i = 0; i < src.size();) {
        const char currCh = src[i];
        if (currCh == '\\' && i + 1 < src.size()) {
            const char nextCh = src[i + 1];
            if (nextCh == 'r') {
                result += '\r';
                i += 2;
                continue;
            }

            if (nextCh == 'n') {
                result += '\n';
                i += 2;
                continue;
            }
        }
        result += currCh;
        ++i;
    }

    return result;
}

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

        auto response = decodeEscapeSeq(line.substr(pos + 1));
        rules.push_back({line.substr(0, pos), response});
    }

    return rules;
}
