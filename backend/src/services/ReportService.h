#pragma once

#include "database/Database.h"
#include "models/Reports.h"

#include <nlohmann/json.hpp>

class ReportService {
public:
    explicit ReportService(Database& database);

    ReportSummary getSummary();
    nlohmann::json getByStatus();
    nlohmann::json getByPriority();
    nlohmann::json getByCategory();
    nlohmann::json getByEngineer();

private:
    int countWhere(const std::string& sql);
    nlohmann::json groupedCount(const std::string& sql, const std::string& labelKey);

    Database& database_;
};
