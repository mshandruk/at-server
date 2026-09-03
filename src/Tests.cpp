#include <cassert>
#include <iostream>

#include "PatternMatcher.hpp"

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

int main() {
    testMatch();
    return 0;
}
