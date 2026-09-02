#include <cstddef>
#include <string>

#include "Match.hpp"

bool match(const std::string& pattern, const std::string& text) {
    if (pattern.empty() && text.empty()) {
        return true;
    }

    std::size_t idxPattern = 0;
    for (std::size_t idxText = 0; idxText < text.size() && idxPattern < pattern.size(); ++idxPattern, ++idxText) {
        if (text[idxText] == pattern[idxPattern]) {
            continue;
        }

        if (pattern[idxPattern] == '.') {
            continue;
        }

        if (pattern[idxPattern] == '*' && idxPattern + 1 >= pattern.size()) {
            return true;
        }
    }

    return (idxPattern > 0 && idxPattern == pattern.size() || pattern[idxPattern] == '*');
}
