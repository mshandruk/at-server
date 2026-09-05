#pragma once

#include <string>
#include <csignal>

#include "RequestHandler.hpp"

class Server {
    public:
        Server(std::string ttyDev, const RequestHandler &requestHandler);
        void start();
    private:
        std::string ttyDev_;
        const RequestHandler& requestHandler_;
        static volatile std::sig_atomic_t shouldRun_;

        static void setupSignal_(bool enable);
        static void handleSignal_(int signum);
};
