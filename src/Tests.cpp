#include <cassert>
#include <cstddef>
#include <iostream>
#include <sstream>

#include "PatternMatcher.hpp"
#include "RulesLoader.hpp"

void testMatch() {
    assert(match("", ""));
    assert(match("AT", "AT"));
    assert(!match("", "AT"));
    assert(!match("AT", "AE"));
    assert(!match("AT", "AT+COPS"));

    assert(match(".", "A"));
    assert(match(".", "ABC"));
    assert(match("A.", "AB"));
    assert(match("A.C", "ABC"));
    assert(!match(".", ""));
    assert(!match("A.", ""));

    assert(match("*", ""));
    assert(match("*", "ABC"));
    assert(match("A*", "ABC"));
    assert(match("A*C", "ABC"));
    assert(match("A*C*E", "ABCDE"));
    assert(match("A*C", "ABC"));
    assert(!match("A*C", "ABE"));
    assert(!match("A*C*E", "ABDDE"));
    assert(!match("A*C*E", "ABCDD"));
    std::cout << "Match tests passed." << '\n';
};

bool operator!=(const AtRule& lhs, const AtRule& rhs) {
    return lhs.commandPattern != rhs.commandPattern && lhs.response != rhs.response;
}

std::ostream& operator<<(std::ostream& oss, const AtRule& rule) {
    return oss << "commandPattern=" << rule.commandPattern << " response=" << rule.response;
}

void testLoadRules() {
    auto testData = std::stringstream("AT=OK\n"
                                      "ATE*=OK\n"
                                      "AT+CPIN?=+CPIN: READY\n"
                                      "AT+COPS.*=OK\n");
    auto rules = loadRules(testData);
    AtRules expectedRules{
        AtRule{"AT", "OK"},
        AtRule{"ATE*", "OK"},
        AtRule{"AT+CPIN?", "+CPIN: READY"},
        AtRule{"AT+COPS.*", "OK"},
    };

    assert(rules.size() == expectedRules.size() && "Diff rules size");
    for (std::size_t i = 0; i < rules.size(); ++i) {
        if (rules[i] != expectedRules[i]) {
            std::cerr << "Recieved: " << rules[i] << '\n';
            std::cerr << "Expected: " << expectedRules[i] << '\n';
        }
    }
    std::cout << "Load rules test passed." << '\n';
}

int main() {
    testMatch();
    testLoadRules();
    return 0;
}
