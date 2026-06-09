#include "models/ActivityLog.h"

namespace {
int toInt(const std::string& value) {
    return value.empty() ? 0 : std::stoi(value);
}
}

nlohmann::json ActivityLog::toJson() const {
    return {
        {"id", id},
        {"ticket_id", ticketId},
        {"action", action},
        {"description", description},
        {"created_at", createdAt}
    };
}

ActivityLog ActivityLog::fromRow(const Database::Row& row) {
    return {
        toInt(row.at("id")),
        toInt(row.at("ticket_id")),
        row.at("action"),
        row.at("description"),
        row.at("created_at")
    };
}
