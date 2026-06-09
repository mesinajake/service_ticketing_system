#include "models/Ticket.h"

namespace {
int toInt(const std::string& value) {
    return value.empty() ? 0 : std::stoi(value);
}
}

nlohmann::json Ticket::toJson() const {
    return {
        {"id", id},
        {"title", title},
        {"description", description},
        {"requester_name", requesterName},
        {"requester_email", requesterEmail},
        {"category", category},
        {"priority", priority},
        {"status", status},
        {"assigned_engineer_id", assignedEngineerId.has_value() ? nlohmann::json(*assignedEngineerId) : nlohmann::json(nullptr)},
        {"assigned_engineer_name", assignedEngineerName.empty() ? nlohmann::json(nullptr) : nlohmann::json(assignedEngineerName)},
        {"resolution_notes", resolutionNotes.has_value() ? nlohmann::json(*resolutionNotes) : nlohmann::json(nullptr)},
        {"created_at", createdAt},
        {"updated_at", updatedAt}
    };
}

Ticket Ticket::fromRow(const Database::Row& row) {
    Ticket ticket;
    ticket.id = toInt(row.at("id"));
    ticket.title = row.at("title");
    ticket.description = row.at("description");
    ticket.requesterName = row.at("requester_name");
    ticket.requesterEmail = row.at("requester_email");
    ticket.category = row.at("category");
    ticket.priority = row.at("priority");
    ticket.status = row.at("status");

    if (!row.at("assigned_engineer_id").empty()) {
        ticket.assignedEngineerId = toInt(row.at("assigned_engineer_id"));
    }

    auto engineerName = row.find("assigned_engineer_name");
    if (engineerName != row.end()) {
        ticket.assignedEngineerName = engineerName->second;
    }

    if (!row.at("resolution_notes").empty()) {
        ticket.resolutionNotes = row.at("resolution_notes");
    }

    ticket.createdAt = row.at("created_at");
    ticket.updatedAt = row.at("updated_at");
    return ticket;
}
