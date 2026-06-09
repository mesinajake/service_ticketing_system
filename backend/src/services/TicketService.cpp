#include "services/TicketService.h"

#include "services/EngineerService.h"
#include "services/LogService.h"
#include "utils/AppError.h"
#include "utils/ErrorCodes.h"
#include "utils/Validator.h"

TicketService::TicketService(Database& database, EngineerService& engineerService, LogService& logService)
    : database_(database), engineerService_(engineerService), logService_(logService) {}

std::vector<Ticket> TicketService::getAllTickets(const TicketFilters& filters) {
    validateFilters(filters);

    std::string sql = ticketSelectSql() + " WHERE 1 = 1";
    std::vector<Database::Parameter> parameters;

    if (!filters.search.empty()) {
        sql += " AND (CAST(t.id AS TEXT) LIKE ? OR t.title LIKE ? OR t.requester_name LIKE ? OR e.full_name LIKE ?)";
        std::string search = "%" + filters.search + "%";
        parameters.insert(parameters.end(), {search, search, search, search});
    }
    if (!filters.status.empty()) {
        sql += " AND t.status = ?";
        parameters.push_back(filters.status);
    }
    if (!filters.priority.empty()) {
        sql += " AND t.priority = ?";
        parameters.push_back(filters.priority);
    }
    if (!filters.category.empty()) {
        sql += " AND t.category = ?";
        parameters.push_back(filters.category);
    }
    if (!filters.dateFrom.empty()) {
        sql += " AND date(t.created_at) >= date(?)";
        parameters.push_back(filters.dateFrom);
    }
    if (!filters.dateTo.empty()) {
        sql += " AND date(t.created_at) <= date(?)";
        parameters.push_back(filters.dateTo);
    }

    sql += " ORDER BY datetime(t.created_at) DESC, t.id DESC";

    if (filters.limit > 0) {
        sql += " LIMIT ?";
        parameters.push_back(std::to_string(filters.limit));
    }

    sql += ";";

    auto rows = database_.queryPrepared(sql, parameters);
    std::vector<Ticket> tickets;
    for (const auto& row : rows) {
        tickets.push_back(Ticket::fromRow(row));
    }
    return tickets;
}

Ticket TicketService::getTicketById(int id) {
    if (!Validator::isValidId(id)) {
        throw AppError(400, ErrorCodes::INVALID_ID);
    }

    auto rows = database_.queryPrepared(ticketSelectSql() + " WHERE t.id = ?;", {std::to_string(id)});
    if (rows.empty()) {
        throw AppError(404, ErrorCodes::TICKET_NOT_FOUND);
    }

    return Ticket::fromRow(rows.front());
}

Ticket TicketService::createTicket(const TicketInput& input) {
    validateTicket(input);

    database_.executePrepared(
        "INSERT INTO tickets (title, description, requester_name, requester_email, category, priority, status, assigned_engineer_id, resolution_notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",
        {
            input.title,
            input.description,
            input.requesterName,
            input.requesterEmail,
            input.category,
            input.priority,
            input.status,
            input.assignedEngineerId.has_value() ? std::optional<std::string>(std::to_string(*input.assignedEngineerId)) : std::nullopt,
            input.resolutionNotes
        }
    );

    int ticketId = database_.lastInsertId();
    logService_.createLog(ticketId, "TICKET_CREATED", "Ticket created: " + input.title);
    return getTicketById(ticketId);
}

Ticket TicketService::updateTicket(int id, const TicketInput& input) {
    validateTicket(input);
    getTicketById(id);

    database_.executePrepared(
        "UPDATE tickets SET title = ?, description = ?, requester_name = ?, requester_email = ?, category = ?, "
        "priority = ?, status = ?, assigned_engineer_id = ?, resolution_notes = ?, updated_at = CURRENT_TIMESTAMP "
        "WHERE id = ?;",
        {
            input.title,
            input.description,
            input.requesterName,
            input.requesterEmail,
            input.category,
            input.priority,
            input.status,
            input.assignedEngineerId.has_value() ? std::optional<std::string>(std::to_string(*input.assignedEngineerId)) : std::nullopt,
            input.resolutionNotes,
            std::to_string(id)
        }
    );

    logService_.createLog(id, "TICKET_UPDATED", "Ticket updated: " + input.title);
    return getTicketById(id);
}

void TicketService::deleteTicket(int id) {
    Ticket ticket = getTicketById(id);
    logService_.createLog(id, "TICKET_DELETED", "Ticket deleted: " + ticket.title);
    database_.executePrepared("DELETE FROM tickets WHERE id = ?;", {std::to_string(id)});
}

Ticket TicketService::updateStatus(int id, const std::string& newStatus) {
    Ticket ticket = getTicketById(id);

    if (!Validator::isValidStatus(newStatus)) {
        throw AppError(400, ErrorCodes::INVALID_STATUS);
    }

    std::optional<std::string> resolutionNotes = ticket.resolutionNotes;
    if (newStatus == "Resolved" && !resolutionNotes.has_value()) {
        resolutionNotes = "Resolved by support engineer.";
    }

    database_.executePrepared(
        "UPDATE tickets SET status = ?, resolution_notes = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?;",
        {newStatus, resolutionNotes, std::to_string(id)}
    );

    std::string action = "TICKET_STATUS_CHANGED";
    if (newStatus == "Resolved") {
        action = "TICKET_RESOLVED";
    } else if (newStatus == "Closed") {
        action = "TICKET_CLOSED";
    }

    logService_.createLog(id, action, "Ticket status changed from " + ticket.status + " to " + newStatus + ".");
    return getTicketById(id);
}

Ticket TicketService::assignEngineer(int id, int engineerId) {
    Ticket ticket = getTicketById(id);
    Engineer engineer = engineerService_.getEngineerById(engineerId);

    database_.executePrepared(
        "UPDATE tickets SET assigned_engineer_id = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?;",
        {std::to_string(engineerId), std::to_string(id)}
    );

    logService_.createLog(id, "TICKET_ASSIGNED", "Ticket assigned to " + engineer.fullName + ": " + ticket.title);
    return getTicketById(id);
}

void TicketService::validateTicket(const TicketInput& input) {
    if (!Validator::isNotEmpty(input.title) ||
        !Validator::isNotEmpty(input.description) ||
        !Validator::isNotEmpty(input.requesterName) ||
        !Validator::isNotEmpty(input.requesterEmail)) {
        throw AppError(400, ErrorCodes::FIELD_REQUIRED);
    }

    if (!Validator::isValidEmail(input.requesterEmail)) {
        throw AppError(400, ErrorCodes::INVALID_EMAIL);
    }
    if (!Validator::isValidPriority(input.priority)) {
        throw AppError(400, ErrorCodes::INVALID_PRIORITY);
    }
    if (!Validator::isValidStatus(input.status)) {
        throw AppError(400, ErrorCodes::INVALID_STATUS);
    }
    if (!Validator::isValidCategory(input.category)) {
        throw AppError(400, ErrorCodes::INVALID_CATEGORY);
    }
    if (input.assignedEngineerId.has_value() && !engineerService_.engineerExists(*input.assignedEngineerId)) {
        throw AppError(404, ErrorCodes::ENGINEER_NOT_FOUND);
    }
}

void TicketService::validateFilters(const TicketFilters& filters) {
    if (!filters.status.empty() && !Validator::isValidStatus(filters.status)) {
        throw AppError(400, ErrorCodes::INVALID_STATUS);
    }
    if (!filters.priority.empty() && !Validator::isValidPriority(filters.priority)) {
        throw AppError(400, ErrorCodes::INVALID_PRIORITY);
    }
    if (!filters.category.empty() && !Validator::isValidCategory(filters.category)) {
        throw AppError(400, ErrorCodes::INVALID_CATEGORY);
    }
}

std::string TicketService::ticketSelectSql() const {
    return
        "SELECT t.id, t.title, t.description, t.requester_name, t.requester_email, t.category, "
        "t.priority, t.status, t.assigned_engineer_id, e.full_name AS assigned_engineer_name, "
        "t.resolution_notes, t.created_at, t.updated_at "
        "FROM tickets t LEFT JOIN engineers e ON e.id = t.assigned_engineer_id";
}
