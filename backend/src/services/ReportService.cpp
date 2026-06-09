#include "services/ReportService.h"

ReportService::ReportService(Database& database)
    : database_(database) {}

ReportSummary ReportService::getSummary() {
    return {
        countWhere("SELECT COUNT(*) AS count FROM tickets;"),
        countWhere("SELECT COUNT(*) AS count FROM tickets WHERE status = 'Open';"),
        countWhere("SELECT COUNT(*) AS count FROM tickets WHERE status = 'In Progress';"),
        countWhere("SELECT COUNT(*) AS count FROM tickets WHERE status = 'Resolved';"),
        countWhere("SELECT COUNT(*) AS count FROM tickets WHERE status = 'Closed';"),
        countWhere("SELECT COUNT(*) AS count FROM tickets WHERE priority = 'Critical';")
    };
}

nlohmann::json ReportService::getByStatus() {
    return groupedCount("SELECT status AS label, COUNT(*) AS count FROM tickets GROUP BY status ORDER BY count DESC;", "status");
}

nlohmann::json ReportService::getByPriority() {
    return groupedCount("SELECT priority AS label, COUNT(*) AS count FROM tickets GROUP BY priority ORDER BY count DESC;", "priority");
}

nlohmann::json ReportService::getByCategory() {
    return groupedCount("SELECT category AS label, COUNT(*) AS count FROM tickets GROUP BY category ORDER BY count DESC;", "category");
}

nlohmann::json ReportService::getByEngineer() {
    return groupedCount(
        "SELECT COALESCE(e.full_name, 'Unassigned') AS label, COUNT(t.id) AS count "
        "FROM tickets t LEFT JOIN engineers e ON e.id = t.assigned_engineer_id "
        "GROUP BY label ORDER BY count DESC;",
        "engineer_name"
    );
}

int ReportService::countWhere(const std::string& sql) {
    auto rows = database_.query(sql);
    return rows.empty() ? 0 : std::stoi(rows.front().at("count"));
}

nlohmann::json ReportService::groupedCount(const std::string& sql, const std::string& labelKey) {
    auto rows = database_.query(sql);
    nlohmann::json result = nlohmann::json::array();

    for (const auto& row : rows) {
        result.push_back({
            {labelKey, row.at("label")},
            {"ticket_count", std::stoi(row.at("count"))},
            {"count", std::stoi(row.at("count"))}
        });
    }

    return result;
}
