#include "models/Engineer.h"

namespace {
int toInt(const std::string& value) {
    return value.empty() ? 0 : std::stoi(value);
}
}

nlohmann::json Engineer::toJson() const {
    nlohmann::json json = {
        {"id", id},
        {"full_name", fullName},
        {"email", email},
        {"role", role},
        {"department", department},
        {"created_at", createdAt}
    };

    if (activeTicketCount.has_value()) {
        json["active_ticket_count"] = *activeTicketCount;
    }

    return json;
}

Engineer Engineer::fromRow(const Database::Row& row) {
    Engineer engineer;
    engineer.id = toInt(row.at("id"));
    engineer.fullName = row.at("full_name");
    engineer.email = row.at("email");
    engineer.role = row.at("role");
    engineer.department = row.at("department");
    engineer.createdAt = row.at("created_at");

    auto count = row.find("active_ticket_count");
    if (count != row.end()) {
        engineer.activeTicketCount = toInt(count->second);
    }

    return engineer;
}
