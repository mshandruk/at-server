#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>

#include "RulesLoader.hpp"
#include "Server.hpp"

int main() {
    std::ifstream ifs("rules.cfg");
    if (!ifs.is_open()) {
        std::cerr << "Cannot open file\n";
        return EXIT_FAILURE;
    }
    auto rules = loadRules(ifs);
    ifs.close();

    try {
        runServer(rules, "/dev/pts/2");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
