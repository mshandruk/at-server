#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "ArgParser.hpp"
#include "RulesLoader.hpp"
#include "Server.hpp"

// NOLINTBEGIN(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-bounds-pointer-arithmetic,readability-implicit-bool-conversion)
int main(int argc, char* argv[]) {
    std::string progname = (argv != nullptr && argv[0] != nullptr) ? argv[0] : "at_server";
    auto programArgs = [&]() {
        try {
            const cli::ArgParser argParser(argc, argv);
            return argParser.parse();
        } catch (const cli::ParseError& e) {
            std::cerr << e.what() << '\n';
            cli::usage(progname);
            std::exit(EXIT_FAILURE);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            std::exit(EXIT_FAILURE);
        }
    }();

    std::ifstream ifs(programArgs.rulesPath);
    if (!ifs.is_open()) {
        std::cerr << "Cannot open file: " + programArgs.rulesPath + '\n';
        return EXIT_FAILURE;
    }

    const auto rules = loadRules(ifs);
    ifs.close();

    try {
        runServer(rules, programArgs.ttyDev);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// NOLINTEND(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-bounds-pointer-arithmetic,readability-implicit-bool-conversion)
