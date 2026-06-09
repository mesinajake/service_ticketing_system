#pragma once

#include <string>

namespace Validator {
    bool isNotEmpty(const std::string& value);
    bool isValidEmail(const std::string& email);
    bool isValidPriority(const std::string& priority);
    bool isValidStatus(const std::string& status);
    bool isValidCategory(const std::string& category);
    bool isValidId(int id);
}
