#pragma once

#include <string>

class RequestHandler {
    public:
        virtual ~RequestHandler() = default;
        virtual std::string handleRequest(const std::string& userCmd) const = 0;
};
