#include <cassert>
#include <iostream>

#include "Match.hpp"

void testMatch() {
    assert(match("", ""));
    assert(!match("", "A"));

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
    std::cout << "Match tests passed." << '\n';
};

int main() {
    testMatch();
    return 0;
}
