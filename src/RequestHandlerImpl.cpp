#include "RequestHandlerImpl.hpp"

#include <string>
#include <utility>

#include "PatternMatcher.hpp"
#include "RulesLoader.hpp"

RequestHandlerImpl::RequestHandlerImpl(AtRules atRules)
        : atRules_(std::move(atRules)) {
}

std::string RequestHandlerImpl::handleRequest(const std::string& request) const {
    for (const auto& rule : atRules_) {
        if (match(rule.commandPattern, request)) {
            return rule.response + "\r\n";
        }
    }
    return "ERROR\r\n";
}
