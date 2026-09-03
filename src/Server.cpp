#include "Server.hpp"

#include <atomic>
#include <cerrno>
#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "PatternMatcher.hpp"
#include "RulesLoader.hpp"

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::atomic<bool> shouldRun = true;

void signalHandler(int signum) {
    if (signum == SIGINT) {
        shouldRun = false;
    }
}

std::string getResponse(const AtRules& rules, const std::string& text) {
    for (const auto& rule : rules) {
        if (match(rule.commandPattern, text)) {
            return rule.response + "\r\n";
        }
    }
    return "ERROR\r\n";
}

void runServer(const AtRules& rules, const std::string& ttyDev) {
    std::signal(SIGINT, signalHandler);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    const int ttyFd = open(ttyDev.c_str(), O_RDWR | O_NOCTTY | O_CLOEXEC);
    if (ttyFd < 0) {
        throw std::runtime_error("Error open tty port: " + ttyDev);
    }

    struct termios tty {};

    if (tcgetattr(ttyFd, &tty) == 0) {
        cfmakeraw(&tty);
        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);
        tty.c_cc[VMIN] = 0;
        tty.c_cc[VTIME] = 1;
        tcsetattr(ttyFd, TCSANOW, &tty);
    }

    std::cout << "At-server started and listen port: " + ttyDev << '\n';
    std::string buffer;

    while (shouldRun) {
        char ch{};
        const ssize_t n = read(ttyFd, &ch, 1);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            std::cerr << "Read error stopping server\n";
            break;
        }
        if (n == 0) {
            continue;
        }

        if (buffer.empty() && (ch == '\r' || ch == '\n')) {
            continue;
        }

        if (ch == '\r' || ch == '\n') {
            auto response = getResponse(rules, buffer);
            write(ttyFd, response.c_str(), response.size());
            tcdrain(ttyFd);
            buffer.clear();
        } else {
            buffer += ch;
        }
    }
    std::cout << "At-server stopped\n";
    close(ttyFd);
}
