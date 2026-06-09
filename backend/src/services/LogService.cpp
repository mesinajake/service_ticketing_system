#include "services/LogService.h"

LogService::LogService(Database& database)
    : database_(database) {}

void LogService::createLog(int ticketId, const std::string& action, const std::string& description) {
    database_.executePrepared(
        "INSERT INTO activity_logs (ticket_id, action, description) VALUES (?, ?, ?);",
        {std::to_string(ticketId), action, description}
    );
}

std::vector<ActivityLog> LogService::getAllLogs() {
    auto rows = database_.query(
        "SELECT id, ticket_id, action, description, created_at "
        "FROM activity_logs ORDER BY datetime(created_at) DESC, id DESC;"
    );

    std::vector<ActivityLog> logs;
    for (const auto& row : rows) {
        logs.push_back(ActivityLog::fromRow(row));
    }
    return logs;
}

std::vector<ActivityLog> LogService::getLogsByTicketId(int ticketId) {
    auto rows = database_.queryPrepared(
        "SELECT id, ticket_id, action, description, created_at "
        "FROM activity_logs WHERE ticket_id = ? ORDER BY datetime(created_at) DESC, id DESC;",
        {std::to_string(ticketId)}
    );

    std::vector<ActivityLog> logs;
    for (const auto& row : rows) {
        logs.push_back(ActivityLog::fromRow(row));
    }
    return logs;
}
