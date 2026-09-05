#pragma once

#include <string>

#include "RequestHandler.hpp"
#include "RulesLoader.hpp"

class RequestHandlerImpl final: public RequestHandler {
    public:
        explicit RequestHandlerImpl(AtRules atRules);
        [[nodiscard]] std::string handleRequest(const std::string& request) const override;
    private:
        AtRules atRules_;
};
