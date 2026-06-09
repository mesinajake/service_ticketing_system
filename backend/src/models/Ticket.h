#pragma once

#include "database/Database.h"

#include <nlohmann/json.hpp>

#include <optional>
#include <string>

struct Ticket {
    int id = 0;
    std::string title;
    std::string description;
    std::string requesterName;
    std::string requesterEmail;
    std::string category;
    std::string priority;
    std::string status;
    std::optional<int> assignedEngineerId;
    std::string assignedEngineerName;
    std::optional<std::string> resolutionNotes;
    std::string createdAt;
    std::string updatedAt;

    nlohmann::json toJson() const;
    static Ticket fromRow(const Database::Row& row);
};
