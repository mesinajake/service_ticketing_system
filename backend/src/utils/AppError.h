#pragma once

#include <stdexcept>
#include <string>

class AppError : public std::runtime_error {
public:
    AppError(int statusCode, const std::string& message)
        : std::runtime_error(message), statusCode_(statusCode) {}

    int statusCode() const {
        return statusCode_;
    }

private:
    int statusCode_;
};
