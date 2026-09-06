#include "RulesLoader.hpp"

#include <cstddef>
#include <iostream>
#include <istream>
#include <string>

std::string decodeEscapeSeq(std::string src) {
    std::size_t writeIdx = 0;
    for (std::size_t readIdx = 0; readIdx < src.size();) {
        const char currCh = src[readIdx];
        if (currCh == '\\' && readIdx + 1 < src.size()) {
            const char nextCh = src[readIdx + 1];
            if (nextCh == 'r') {
                src[writeIdx++] = '\r';
                readIdx += 2; // skip currCh and nextCh
                continue;
            }

            if (nextCh == 'n') {
                src[writeIdx++] = '\n';
                readIdx += 2;
                continue;
            }
        }
        src[writeIdx++] = src[readIdx++];
    }

    src.resize(writeIdx);
    return src;
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
