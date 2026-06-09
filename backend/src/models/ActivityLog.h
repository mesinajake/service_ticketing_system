#pragma once

#include "database/Database.h"

#include <nlohmann/json.hpp>

#include <string>

struct ActivityLog {
    int id = 0;
    int ticketId = 0;
    std::string action;
    std::string description;
    std::string createdAt;

    nlohmann::json toJson() const;
    static ActivityLog fromRow(const Database::Row& row);
};
