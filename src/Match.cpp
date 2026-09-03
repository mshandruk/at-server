#include <cstddef>
#include <string>

#include "Match.hpp"

bool match(const std::string& pattern, const std::string& text) {
    if (pattern.empty() && text.empty()) {
        return true;
    }

    if (pattern.empty()) {
        return false;
    }

    std::size_t idxPattern = 0;
    std::size_t idxText = 0;

    for (; idxText < text.size() && idxPattern < pattern.size(); ++idxText, ++idxPattern) {
        if (text[idxText] == pattern[idxPattern]) {
            continue;
        }

        const auto patternSymbol = pattern[idxPattern];
        if (patternSymbol == '.' || patternSymbol == '*' && idxPattern + 1 >= pattern.size()) {
            return true;
        }

        if (patternSymbol == '.' || patternSymbol == '*') {
            continue;
        }

        return false;
    }
    return idxPattern == pattern.size() && idxText == text.size() || pattern[idxPattern] == '*';
}
