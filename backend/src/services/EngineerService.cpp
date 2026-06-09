#include "services/EngineerService.h"

#include "utils/AppError.h"
#include "utils/ErrorCodes.h"
#include "utils/Validator.h"

EngineerService::EngineerService(Database& database)
    : database_(database) {}

std::vector<Engineer> EngineerService::getAllEngineers() {
    auto rows = database_.query(
        "SELECT e.id, e.full_name, e.email, e.role, e.department, e.created_at, "
        "COUNT(t.id) AS active_ticket_count "
        "FROM engineers e "
        "LEFT JOIN tickets t ON t.assigned_engineer_id = e.id AND t.status IN ('Open', 'In Progress') "
        "GROUP BY e.id "
        "ORDER BY e.full_name ASC;"
    );

    std::vector<Engineer> engineers;
    for (const auto& row : rows) {
        engineers.push_back(Engineer::fromRow(row));
    }
    return engineers;
}

Engineer EngineerService::getEngineerById(int id) {
    if (!Validator::isValidId(id)) {
        throw AppError(400, ErrorCodes::INVALID_ID);
    }

    auto rows = database_.queryPrepared(
        "SELECT e.id, e.full_name, e.email, e.role, e.department, e.created_at, "
        "COUNT(t.id) AS active_ticket_count "
        "FROM engineers e "
        "LEFT JOIN tickets t ON t.assigned_engineer_id = e.id AND t.status IN ('Open', 'In Progress') "
        "WHERE e.id = ? GROUP BY e.id;",
        {std::to_string(id)}
    );

    if (rows.empty()) {
        throw AppError(404, ErrorCodes::ENGINEER_NOT_FOUND);
    }

    return Engineer::fromRow(rows.front());
}

Engineer EngineerService::createEngineer(const EngineerInput& input) {
    validateEngineer(input);

    auto duplicateRows = database_.queryPrepared(
        "SELECT id FROM engineers WHERE email = ?;",
        {input.email}
    );

    if (!duplicateRows.empty()) {
        throw AppError(400, ErrorCodes::EMAIL_ALREADY_EXISTS);
    }

    database_.executePrepared(
        "INSERT INTO engineers (full_name, email, role, department) VALUES (?, ?, ?, ?);",
        {input.fullName, input.email, input.role, input.department}
    );

    return getEngineerById(database_.lastInsertId());
}

bool EngineerService::engineerExists(int id) {
    auto rows = database_.queryPrepared("SELECT id FROM engineers WHERE id = ?;", {std::to_string(id)});
    return !rows.empty();
}

void EngineerService::validateEngineer(const EngineerInput& input) {
    if (!Validator::isNotEmpty(input.fullName) ||
        !Validator::isNotEmpty(input.email) ||
        !Validator::isNotEmpty(input.role) ||
        !Validator::isNotEmpty(input.department)) {
        throw AppError(400, ErrorCodes::FIELD_REQUIRED);
    }

    if (!Validator::isValidEmail(input.email)) {
        throw AppError(400, ErrorCodes::INVALID_EMAIL);
    }
}
