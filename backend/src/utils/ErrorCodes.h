#pragma once

#include <string>

namespace ErrorCodes {
    const std::string FIELD_REQUIRED = "Field is required and cannot be empty.";
    const std::string INVALID_EMAIL = "Invalid email format.";
    const std::string INVALID_PRIORITY = "Priority must be: Low, Medium, High, or Critical.";
    const std::string INVALID_STATUS = "Status must be: Open, In Progress, Resolved, or Closed.";
    const std::string INVALID_CATEGORY = "Category must be one of the defined values.";
    const std::string TICKET_NOT_FOUND = "Ticket not found.";
    const std::string ENGINEER_NOT_FOUND = "Engineer not found.";
    const std::string EMAIL_ALREADY_EXISTS = "An engineer with this email already exists.";
    const std::string INVALID_ID = "ID must be a valid positive integer.";
}
