#pragma once

#include <string>
#include <csignal>
#include <atomic>

#include "RequestHandler.hpp"

class Server {
    public:
        Server(std::string ttyDev, const RequestHandler &requestHandler);
        void start();
    private:
        std::string ttyDev_;
        const RequestHandler& requestHandler_;
        static std::atomic<bool> stopRequested_;
};
