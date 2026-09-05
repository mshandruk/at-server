#include "ArgParser.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace cli {

void usage(const std::string& progname) {
    std::cout << "Example usage: " << progname << " --rules <rules.cfg>" << " --tty </dev/pts/<N>>" << '\n';
}

// NOLINTBEGIN(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-bounds-pointer-arithmetic)
ArgParser::ArgParser(int argc, char* argv[]) {
    if (argc < 2) {
        throw ParseError("Invalid number arguments");
    };

    arguments_.reserve(argc - 1);
    for (int i = 1; i < argc; ++i) {
        if (argv[i] != nullptr) {
            arguments_.emplace_back(argv[i]);
        }
    }
}

// NOLINTEND(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-bounds-pointer-arithmetic)

ProgramArgs ArgParser::parse() const {
    ProgramArgs programArgs{};

    for (std::size_t i = 0; i < arguments_.size();) {
        const std::string argument = arguments_[i];
        if (argument == "--rules") {
            programArgs.rulesPath = getNextValue(i, argument);
            i += 2;
            continue;
        }

        if (argument == "--tty") {
            programArgs.ttyDev = getNextValue(i, argument);
            i += 2;
            continue;
        }

        throw ParseError("Unknown option: " + argument);
    }

    if (programArgs.rulesPath.empty()) {
        throw ParseError("Missing required option: --rules");
    }

    if (programArgs.ttyDev.empty()) {
        throw ParseError("Missing required option: --tty");
    }

    return programArgs;
}

std::string ArgParser::getNextValue(std::size_t currIdx, const std::string& argument) const {
    if (currIdx + 1 >= arguments_.size()) {
        throw ParseError("Option " + argument + " requires an argument");
    }

    const std::string value = arguments_[currIdx + 1];
    if (value.rfind('-', 0) == 0) {
        throw ParseError("Option " + argument + " requires an argument, but '" + value + "' was given");
    }

    return value;
}

} // namespace cli
