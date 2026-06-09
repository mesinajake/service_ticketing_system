#pragma once

#include "database/Database.h"
#include "models/Ticket.h"

#include <optional>
#include <string>
#include <vector>

class EngineerService;
class LogService;

struct TicketInput {
    std::string title;
    std::string description;
    std::string requesterName;
    std::string requesterEmail;
    std::string category;
    std::string priority;
    std::string status;
    std::optional<int> assignedEngineerId;
    std::optional<std::string> resolutionNotes;
};

struct TicketFilters {
    std::string search;
    std::string status;
    std::string priority;
    std::string category;
    std::string dateFrom;
    std::string dateTo;
    int limit = 0;
};

class TicketService {
public:
    TicketService(Database& database, EngineerService& engineerService, LogService& logService);

    std::vector<Ticket> getAllTickets(const TicketFilters& filters);
    Ticket getTicketById(int id);
    Ticket createTicket(const TicketInput& input);
    Ticket updateTicket(int id, const TicketInput& input);
    void deleteTicket(int id);
    Ticket updateStatus(int id, const std::string& newStatus);
    Ticket assignEngineer(int id, int engineerId);

private:
    void validateTicket(const TicketInput& input);
    void validateFilters(const TicketFilters& filters);
    std::string ticketSelectSql() const;

    Database& database_;
    EngineerService& engineerService_;
    LogService& logService_;
};
