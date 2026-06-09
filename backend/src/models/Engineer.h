#pragma once

#include "database/Database.h"

#include <nlohmann/json.hpp>

#include <optional>
#include <string>

struct Engineer {
    int id = 0;
    std::string fullName;
    std::string email;
    std::string role;
    std::string department;
    std::string createdAt;
    std::optional<int> activeTicketCount;

    nlohmann::json toJson() const;
    static Engineer fromRow(const Database::Row& row);
};
