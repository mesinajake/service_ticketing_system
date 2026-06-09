#include "utils/Validator.h"

#include <regex>
#include <set>

namespace Validator {

bool isNotEmpty(const std::string& value) {
    return !value.empty() && value.find_first_not_of(" \t\r\n") != std::string::npos;
}

bool isValidEmail(const std::string& email) {
    static const std::regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return std::regex_match(email, pattern);
}

bool isValidPriority(const std::string& priority) {
    static const std::set<std::string> allowed = {"Low", "Medium", "High", "Critical"};
    return allowed.count(priority) > 0;
}

bool isValidStatus(const std::string& status) {
    static const std::set<std::string> allowed = {"Open", "In Progress", "Resolved", "Closed"};
    return allowed.count(status) > 0;
}

bool isValidCategory(const std::string& category) {
    static const std::set<std::string> allowed = {
        "Software Issue",
        "Hardware Issue",
        "Network Issue",
        "Account Access",
        "System Bug",
        "Client Request",
        "Others"
    };
    return allowed.count(category) > 0;
}

bool isValidId(int id) {
    return id > 0;
}

}
