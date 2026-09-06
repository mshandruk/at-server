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
#include <utility>

#include "RequestHandler.hpp"

class FileDescriptor {
  public:
    explicit FileDescriptor(const std::string& filePath, int flags)
            : fd_{::open(filePath.c_str(), flags)} { // NOLINT

        if (fd_ < 0) {
            throw std::runtime_error("Failed to open: " + filePath);
        }
    }

    [[nodiscard]] int get() const {
        return fd_;
    }

    ~FileDescriptor() {
        if (fd_ >= 0) {
            ::close(fd_);
        }
    }

    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    FileDescriptor(FileDescriptor&&) = default;
    FileDescriptor& operator=(FileDescriptor&&) = default;

  private:
    int fd_;
};

class SignalHandlerGuard {
  public:
    explicit SignalHandlerGuard(std::atomic<bool>& stopRequested) {
        stopRequested.store(false);
        activeStopRequested_ = &stopRequested;
        setupSignal_(true);
    }

    ~SignalHandlerGuard() {
        setupSignal_(false);
        activeStopRequested_ = nullptr;
    }

    SignalHandlerGuard(const SignalHandlerGuard&) = delete;
    SignalHandlerGuard& operator=(const SignalHandlerGuard&) = delete;

    SignalHandlerGuard(SignalHandlerGuard&&) = delete;
    SignalHandlerGuard& operator=(SignalHandlerGuard&&) = delete;

  private:
    static std::atomic<bool>* activeStopRequested_;

    static void handleSignal_(int signum) {
        if (signum == SIGINT && activeStopRequested_ != nullptr) {
            SignalHandlerGuard::activeStopRequested_->store(true);
        }
    }

    static void setupSignal_(bool enable) {
        struct sigaction sa {};

        if (enable) {
            sa.sa_handler = &handleSignal_;
        } else {
            sa.sa_handler = SIG_DFL;
        }
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, nullptr);
    }
};

std::atomic<bool>* SignalHandlerGuard::activeStopRequested_{nullptr};

std::atomic<bool> Server::stopRequested_{false};

Server::Server(std::string ttyDev, const RequestHandler& handler)
        : ttyDev_{std::move(ttyDev)},
          requestHandler_{handler} {
}

void Server::start() {
    const FileDescriptor ttyFd(ttyDev_, O_RDWR | O_NOCTTY | O_CLOEXEC);

    struct termios tty {};

    if (::tcgetattr(ttyFd.get(), &tty) != 0) {
        throw std::runtime_error("tcgetattr failed");
    }

    ::cfmakeraw(&tty);
    ::cfsetispeed(&tty, B115200);
    ::cfsetospeed(&tty, B115200);

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1; // Unblock every 100ms.

    ::tcsetattr(ttyFd.get(), TCSANOW, &tty);

    const SignalHandlerGuard signalGuard(stopRequested_);

    std::cout << "AT-server started and listen port: " + ttyDev_ << '\n';
    std::string buffer;
    while (!stopRequested_.load()) {
        char ch;
        const ssize_t n = ::read(ttyFd.get(), &ch, 1);

        if (n < 0) {
            if (errno == EINTR) {
                std::cerr << "Signal interrupt\n";
                continue;
            }
            std::cerr << "Read error occurred\n";
            break;
        }

        if (n == 0) {
            continue;
        }

        if (buffer.empty() && (ch == '\r' || ch == '\n')) {
            continue;
        }

        if (ch == '\r' || ch == '\n') {
            auto response = requestHandler_.handleRequest(buffer);
            if (!response.empty()) {
                write(ttyFd.get(), response.c_str(), response.size());
                tcdrain(ttyFd.get());
            }
            buffer.clear();
        } else {
            buffer += ch;
        }
    }
    std::cout << "AT-server stopped\n";
}
