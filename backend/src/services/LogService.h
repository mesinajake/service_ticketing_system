#pragma once

#include "database/Database.h"
#include "models/ActivityLog.h"

#include <string>
#include <vector>

class LogService {
public:
    explicit LogService(Database& database);

    void createLog(int ticketId, const std::string& action, const std::string& description);
    std::vector<ActivityLog> getAllLogs();
    std::vector<ActivityLog> getLogsByTicketId(int ticketId);

private:
    Database& database_;
};
