#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace cli {

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct ProgramArgs {
    std::string rulesPath;
    std::string ttyDev;
};

void usage(const std::string& progname);

class ArgParser {
  public:
    // NOLINTBEGIN(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-bounds-pointer-arithmetic)
    ArgParser(int argc, char* argv[]);
    // NOLINTEND(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-bounds-pointer-arithmetic)

    [[nodiscard]] ProgramArgs parse() const;

  private:
    std::vector<std::string> arguments_;

    [[nodiscard]] std::string getNextValue(std::size_t currIdx, const std::string& argument) const;
};

} // namespace cli
